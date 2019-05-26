
#include "SALB.h"

#include <string>
#include <memory>
#include <android/log.h>

#include "engine/manager/include/TTSManager.h"
#include "engine/manager/include/common.h"

#define DEBUG_TAG "NDK_Synthesis"


static htstts::TTSManager tts;                     //- our TTS using default synthesizer and text analysis
static htstts::FragmentPropertiesPtr properties = nullptr;

JNIEXPORT jshortArray JNICALL Java_at_ftw_speechsynthesis_FTWTTSEngine_nativeSynthesize(JNIEnv* env, jobject thisObj, jstring utterance, jstring rate, jstring path,jstring jLanguageType) {
    jboolean isCopy;
    const char* szUtterance = env->GetStringUTFChars(utterance, &isCopy);
    const char* szRate = env->GetStringUTFChars(rate, &isCopy);
    const char* szPath = env->GetStringUTFChars(path, &isCopy);
    std::string languageType = env->GetStringUTFChars(jLanguageType,&isCopy);
    std::string languageText = "language set: "+languageType;
    try {
        //================== Setting properties ====================
        __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Setting synthesis properties");

        if( properties == nullptr ) {
            properties = htstts::FragmentPropertiesPtr( new htstts::FragmentProperties() );
            (*properties)[PROPERTY_KEY_TEXTANALYZER] = PROPERTY_VALUE_INTERNAL;                                     //- use internal text analyzer

            if(languageType.compare("de-at")==0)
            {
                __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "de-at");
                (*properties)[PROPERTY_KEY_LANGUAGE] = "de-at";                                                  //- voice language
            }
            else
            {
                __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "en-us");
                (*properties)[PROPERTY_KEY_LANGUAGE] ="en-US";
            }
            (*properties)[PROPERTY_KEY_SYNTHESIZER] =  PROPERTY_VALUE_HTSENGINE;                                    //- use hts_engine as synthesizer
            (*properties)[PROPERTY_KEY_VOICE_NAME]  = "Leopold";                                                    //- voice name
            (*properties)[PROPERTY_KEY_VOLUME]      = "90";                                                         //- voice volume 0-100
        }
        if(languageType.compare("de-at")==0)
        {
            __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "de-at");
            (*properties)[PROPERTY_KEY_TEXTANALYZER_RULES] = std::string(szPath) + "/leo.rules";
            (*properties)[PROPERTY_KEY_VOICE_PATH] = std::string(szPath) + "/leo.htsvoice";
        }
        else
        {
            __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "en-us");
            (*properties)[PROPERTY_KEY_TEXTANALYZER_RULES] = std::string(szPath) + "/mini.rules";
            (*properties)[PROPERTY_KEY_VOICE_PATH] = std::string(szPath) + "/cmu_us_arctic_slt.htsvoice";
        }
        (*properties)[PROPERTY_KEY_RATE]        = szRate;


        //================== Synthesizing ====================
        __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Synthesizing");

        htstts::TextFragmentPtr tf = htstts::TextFragmentPtr( new htstts::TextFragment( std::string(szUtterance), properties ) );
        htstts::TTSResultPtr result = tts.SynthesizeTextFragment( tf );

        __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Synthesizing done");

        //================== Converting data ====================
        std::vector<double>& samples = result->GetFrames();
        jshortArray resultarray = env->NewShortArray(samples.size());

        if (resultarray != NULL) {
            jshort *fill = env->GetShortArrayElements(resultarray, NULL);
            double x;
            short temp;

            // write data to java array, clamp to short
            for (int i = 0; i < samples.size(); i++) {
                x = samples[i];
                if (x > 32767.0)        temp = 32767;
                else if (x < -32768.0)  temp = -32768;
                else                    temp = (short) x;
                fill[i] = temp;
            }
            env->ReleaseStringUTFChars(path, szPath);
            env->ReleaseStringUTFChars(rate, szRate);
            env->ReleaseStringUTFChars(utterance, szUtterance);
            env->ReleaseShortArrayElements(resultarray, fill, 0);

            properties = nullptr;
            return resultarray;
        }
    }
    catch( const htstts::PropertyMissingException& e ) {
        __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Property missing [%s]", e.what());
    }
    catch( const htstts::FileLoadException& e ) {
        __android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "Could not load file: [%s]", e.what());
    }

    env->ReleaseStringUTFChars(path, szPath);
    env->ReleaseStringUTFChars(rate, szRate);
    env->ReleaseStringUTFChars(utterance, szUtterance);
    return NULL;

}

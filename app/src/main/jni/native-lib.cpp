#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring

JNICALL
Java_more_dsi_iii_com_hts_1tts_1test2_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */)
{
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

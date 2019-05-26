/**
 * SALB TTS
 *
 * @author Markus Toman, 2015
 */

package at.ftw.speechsynthesis;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;
import android.content.res.AssetManager;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

/**
 * The TTS Engine wraps native code for HMM-based speech synthesis.
 */
public class FTWTTSEngine
{
    
    private static final int MODEL_FREQUENCY = 48000;
    private String ttsFileDirName = null;
    private String modelPath;
    private Context mContext = null;
    
    /**
     * Instantiates a new FTWTTS engine.
     *
     * @param context the context
     */
    public FTWTTSEngine(Context context, String ttsFileDirName)
    {
        this.ttsFileDirName = ttsFileDirName;
        this.mContext = context;
        Log.i("FTWTTSEngine", "Copying assets.");
        copyAssets(context);
        
    }
    
    
    public synchronized void synthesize(String txt, String languageType)
    {
        
        short[] cache = nativeSynthesize(txt, "1.0", mContext.getFilesDir().getAbsolutePath() + "/" +
                modelPath, languageType);
        
        AudioTrack audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, MODEL_FREQUENCY, AudioFormat
                .CHANNEL_OUT_MONO, AudioFormat.ENCODING_PCM_16BIT, cache.length * 2, AudioTrack.MODE_STATIC);
        
        audioTrack.write(cache, 0, cache.length);
        audioTrack.play();
    }
    
    /**
     * Copies assets needed for speech synthesis to the data folder.
     *
     * @param context the context used
     */
    private synchronized void copyAssets(Context context)
    {
        
        modelPath = ttsFileDirName + "/";
        Log.i("modelPath", modelPath);
        copyFile(context, "cmu_us_arctic_slt.htsvoice", modelPath);
        copyFile(context, "mini.rules", modelPath);
        copyFile(context, "leo.htsvoice", modelPath);
        copyFile(context, "leo.rules", modelPath);
    }
    
    /**
     * Copy file.
     *
     * @param context  the context
     * @param filename the filename
     */
    private synchronized void copyFile(Context context, String filename, String modelPath)
    {
        AssetManager assetManager = context.getAssets();
        
        InputStream in = null;
        OutputStream out = null;
        String newFileName = null;
        try
        {
            in = assetManager.open(modelPath + filename);
            
            newFileName = modelPath + filename;
            Log.d("newFileName", newFileName);
            
            final File newFile = new File(mContext.getFilesDir().getAbsolutePath() + "/" + modelPath);
            newFile.mkdir();
            
            out = new FileOutputStream(mContext.getFilesDir().getAbsolutePath() + "/" + newFileName);
            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1)
            {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
        }
        catch (Exception e)
        {
            Log.e("ftw", "Exception in copyFile() " + e.toString());
        }
    }
    
    
    /**
     * NDK call to native speech synthesis library.
     *
     * @param utterance utterance to speak.
     * @return the short[]
     */
    private native short[] nativeSynthesize(String utterance, String rate, String path, String jLanguageType);
    
    static
    {
        System.loadLibrary("TTS");
    }
}

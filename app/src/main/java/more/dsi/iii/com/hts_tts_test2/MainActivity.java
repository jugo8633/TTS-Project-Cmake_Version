package more.dsi.iii.com.hts_tts_test2;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.TextView;

import at.ftw.speechsynthesis.FTWTTSEngine;

public class MainActivity extends AppCompatActivity
{
    private FTWTTSEngine mFTWTTSEngine = null;
    private EditText mEditText = null;
    private Button mButton = null;
    private String whichLanguage = "en-us";
    private RadioGroup languageGroup = null;
    
    
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mFTWTTSEngine = new FTWTTSEngine(this, "eng_tts_data");
        
        languageGroup = (RadioGroup) findViewById(R.id.rgroup);
        languageGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener()
        {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, int i)
            {
                switch (i)
                {
                    case R.id.radio_eng:
                        whichLanguage = "en-us";
                        break;
                    case R.id.radio_de:
                        whichLanguage = "de-at";
                        break;
                    default:
                        whichLanguage = "en-us";
                        break;
                }
            }
        });
        mEditText = (EditText) findViewById(R.id.editText);
        mButton = (Button) findViewById(R.id.button);
        
        mButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                Log.i("languageType", String.valueOf(whichLanguage));
                new Thread(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        mFTWTTSEngine.synthesize(mEditText.getText().toString(), whichLanguage);
                    }
                }).start();
                
            }
        });
        
        
    }
    
    
}

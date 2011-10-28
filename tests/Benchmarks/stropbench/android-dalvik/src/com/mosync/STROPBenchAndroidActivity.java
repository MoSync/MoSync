package com.mosync;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class STROPBenchAndroidActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        TextView tv = (TextView) findViewById(R.id.textview);
        //setContentView(tv);
         
        
        
        //Do the benching!
        StropBencher sb = new StropBencher(tv);
        sb.execute();
//        tv.append(sb.bench(StropBencher.APPEND));
//        tv.invalidate();
//        tv.append(sb.bench(StropBencher.COPY));
//        tv.invalidate();
//        tv.append(sb.bench(StropBencher.FIND));
//        tv.invalidate();
//        tv.append(sb.bench(StropBencher.SUBSTR));
//        tv.invalidate();
//        tv.append(sb.bench(StropBencher.INSERT));
//        tv.invalidate();
//        tv.append(sb.bench(StropBencher.REMOVE));
//        tv.invalidate();
//        tv.append(sb.bench(StropBencher.RESIZE));
//        tv.invalidate();
//        tv.append(sb.bench(StropBencher.COMPARE));
    
    }
    
}
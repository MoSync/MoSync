package com.mosync;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

public class ResultsActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.results); //show the results screen! :)
        
        mData = Data.getInstance(this.getBaseContext());
        
        TextView tv = (TextView) findViewById(R.id.tv);
        tv.setText(mData.getResults());
        
    }
    
    Data mData;

}
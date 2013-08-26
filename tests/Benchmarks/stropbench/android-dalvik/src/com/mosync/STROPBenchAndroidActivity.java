/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

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
//		  tv.append(sb.bench(StropBencher.APPEND));
//		  tv.invalidate();
//		  tv.append(sb.bench(StropBencher.COPY));
//		  tv.invalidate();
//		  tv.append(sb.bench(StropBencher.FIND));
//		  tv.invalidate();
//		  tv.append(sb.bench(StropBencher.SUBSTR));
//		  tv.invalidate();
//		  tv.append(sb.bench(StropBencher.INSERT));
//		  tv.invalidate();
//		  tv.append(sb.bench(StropBencher.REMOVE));
//		  tv.invalidate();
//		  tv.append(sb.bench(StropBencher.RESIZE));
//		  tv.invalidate();
//		  tv.append(sb.bench(StropBencher.COMPARE));

	}

}
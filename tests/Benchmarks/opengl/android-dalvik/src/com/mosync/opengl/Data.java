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

package com.mosync.opengl;

import android.content.Context;

/*
 * Singleton class for sharing the result string between Activities
 */
final class Data {

	private static Context context = null;
	String resultString;

	private Data() {
		resultString = new String();
	}

	/*
	 * Return the result string
	 */
	public String getResults() {
		return resultString;
	}

	/*
	 * Append to the result string
	 */
	public void addResult(String str) {
		resultString = resultString.concat(str);
	}

	private static class Singleton {
        private static final Data INSTANCE = new Data();
    }

	/**
     *  a singleton - do not use new(), use getInstance().
     */
    static synchronized Data getInstance(Context c) {
        if (context == null) context = c.getApplicationContext(); //singleton for this application
        return Singleton.INSTANCE;
    }


}

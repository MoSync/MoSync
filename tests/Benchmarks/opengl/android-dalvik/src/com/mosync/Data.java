package com.mosync;

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

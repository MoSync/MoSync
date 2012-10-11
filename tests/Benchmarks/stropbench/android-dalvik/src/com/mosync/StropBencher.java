package com.mosync;

import java.util.Random;

import android.os.AsyncTask;
import android.os.SystemClock;
import android.widget.TextView;

public class StropBencher extends AsyncTask<Void, Integer, Void>{

	/* The different benchmarks */
	static final int APPEND = 0; 
	static final int COPY = 1;
	static final int FIND = 2;
	static final int SUBSTR = 3;
	static final int INSERT = 4;
	static final int REMOVE = 5;
	static final int RESIZE = 6;
	static final int COMPARE = 7;

	/* The different String types */
	static final int JAVA_LANG_STRING = 0;

	/* used as number of iterations when we want to do a lot of operations */
	static final int ALOT = 256;

	/* Other constants */
	static final int RUNNING_TIME = 1000; // msecs
	
	/* members */
	TextView mTv;
	String mResults;
	
	/* Constructor */
	public StropBencher(TextView tv) {
		mTv = tv;
		mResults = "";
	}
	
	@Override
	protected Void doInBackground(Void... params) {
		
		mResults = bench(StropBencher.APPEND);
		publishProgress(100/8);
		mResults = bench(StropBencher.COPY);
        publishProgress(100/8*2);
        mResults = bench(StropBencher.FIND);
        publishProgress(100/8*3);
        mResults = bench(StropBencher.SUBSTR);
        publishProgress(100/8*4);
        mResults = bench(StropBencher.INSERT);
        publishProgress(100/8*5);
        mResults = bench(StropBencher.REMOVE);
        publishProgress(100/8*6);
        mResults = bench(StropBencher.RESIZE);
        publishProgress(100/8*7);
        mResults = bench(StropBencher.COMPARE);
        publishProgress(100);
        
        return null;
		
	}
	
	@Override
	protected void onProgressUpdate(Integer... progress) {
    	
		mTv.append("===============\n");
		mTv.append(mResults);
		mTv.append(progress[0] + "% done\n");
		
    }

	/* Performs a benchmark according to @param benchmark */
	public String bench(int benchmark) {

		long time;
		long i;
		String ret = "";

		switch(benchmark){
		case APPEND:
			//double the amount of test runs until the test take more than RUNNING_TIME to perform
			for(i = 1; (time = this.appender(i, JAVA_LANG_STRING, 0)) < RUNNING_TIME; i*=2);
			ret += "appending " + ALOT*i + " java.lang.Strings to a java.lang.String in ";
			ret += "Time: " + time + " msecs, " + (float) (ALOT*i)/(float) time + " KSTROPS.\n";

			for(i = 1; (time = this.appender(i, JAVA_LANG_STRING, 1)) < RUNNING_TIME; i*=2);
			ret += "appending " + ALOT*i + " java.lang.Strings to a java.lang.String in";
			ret += "Time: " + time + " msecs, " + (float) (ALOT*i)/(float) time + " KSTROPS.\n";
			break;

		case COPY:
			for(i = 1; (time = this.copy(i, JAVA_LANG_STRING)) < RUNNING_TIME; i*=2);
			ret += "copying " + ALOT*i + " java.lang.Strings to a java.lang.String in ";
			ret += "Time: " + time + " msecs, " + (float) (ALOT*i)/(float) time + " KSTROPS.\n";
			break;

		case FIND:
			for(i = 1; (time = this.find(i, JAVA_LANG_STRING, 0)) < RUNNING_TIME; i*=2);
			ret += "searching for each character in the alphabet " + ALOT*i + " times using find() (or equivalent)";
			ret += "Time: " + time + " msecs, " + (float) (ALOT*i)/(float) time + " KSTROPS.\n";

			for(i = 1; (time = this.find(i, JAVA_LANG_STRING, 1)) < RUNNING_TIME; i*=2);
			ret += "searching for each character in the alphabet " + ALOT*i + " times using findLastOf() (or equivalent)";
			ret += "Time: " + time + " msecs, " + (float) (ALOT*i)/(float) time + " KSTROPS.\n";
			break;

		case SUBSTR:
			for(i = 1; (time = this.substr(i, JAVA_LANG_STRING)) < RUNNING_TIME; i*=2);
			ret += "substring a java.lang.String and store in another java.lang.String " + ALOT*i + " times";
			ret += "Time: " + time + " msecs, " + (float) (ALOT*i)/(float) time + " KSTROPS.\n";
			break;

		case INSERT:
			ret += "java.lang.String have no function that is equivalent to insert()\n";
			break;
		case REMOVE:
			ret += "java.lang.String have no function that is equivalent to remove()\n";
			break;
		case RESIZE:
			ret += "java.lang.String have no function that is equivalent to remove()\n";
			break;

		case COMPARE:
			for(i = 1; (time = this.compare(i, JAVA_LANG_STRING, 0)) < RUNNING_TIME; i*=2);
			ret += "comparing java.lang.String with another " + ALOT*i + " times";
			ret += "Time: " + time + " msecs,  " + (float) (ALOT*i)/(float) time + " KSTROPS.\n";
			
			for(i = 1; (time = this.compare(i, JAVA_LANG_STRING, 1)) < RUNNING_TIME; i*=2);
			ret += "comparing java.lang.String with another " + ALOT*i + " times";
			ret += "Time: " + time + " msecs,  " + (float) (ALOT*i)/(float) time + " KSTROPS.\n";
			break;

		}		
		return ret;
	}

	/*
	 * Bench string appending
	 */
	long appender(long numRuns, int strType, int func) {

		long startTime = currTime();
		String str = "Ima String!";
		String appStr = "Append me!";
		switch(strType) {

		case JAVA_LANG_STRING:

			for(int i = 0; i < numRuns; ++i){
				for(int j = 0; j < ALOT; ++j){
					if(func == 0)
						appStr.concat("junk");
					else if(func == 1)
						appStr += str; //testing operator+=
				}
			}

			/*case STD_STRING: TODO Will have to wait until STL is implemented

			for(int i = 0; i < numRuns; ++i){
				for(int j = 0; j < ALOT; ++j){
					mStdStr->append("junk");
				}
			}
			return currTime() - startTime;*/

		}

		return currTime() - startTime;
	}

	/*
	 * copy 2 strings back and forth
	 */
	long copy(long numRuns, int strType) { //TODO rewrite, this only copy pointers

		long startTime = currTime();
		String tmp;
		String cpyStr = "junk";
		String cpyStr2 = "more junk";
		switch(strType) {

		case JAVA_LANG_STRING:

			for(int i = 0; i < numRuns; ++i){
				for(int j = 0; j < ALOT; ++j){
					tmp = cpyStr2;
					cpyStr2 = cpyStr;
					cpyStr = tmp;
				}
			}
			System.out.println("cpyStr = " + cpyStr); //force the compiler to do the ops since cpyStr is used later on

			/*case STD_STRING: TODO Will have to wait until STL is implemented*/

		}

		return currTime() - startTime;

	}

	/*
	 * Benchmark the find functions family (On Android they are actually named indexOf!)
	 * @param func tells which find-function to bench
	 */
	long find(long numRuns, int strType, int func) {

		long startTime = currTime();
		String alphabet = "abcdefghiljkmnopqrstuvwxyz";
		//const char * c_str = "a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0k\0l\0m\0n\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z\0";
		String largeAlphabet = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz";
		int res; //result of find (position where char was found or -1 on no match)

		switch(strType) {

		case JAVA_LANG_STRING:

			for(int i = 0; i < numRuns; ++i){
				for(int j = 0; j < ALOT; ++j){
					for(char k = 0; k < alphabet.length(); ++k){
						if(func == 0){
							res = alphabet.indexOf((char) k+97); 
							if(res == -1)
								System.err.println("Error in find(), no match but it should be!");
						}else if(func == 1){
							res = largeAlphabet.lastIndexOf((char) k+97); //ascii val of 'a' is 97
							if(res == -1)
								System.err.println("Error in findLastOf(), no match but it should be!");
						}
					}
				}
			}

			/*case STD_STRING: TODO Will have to wait until STL is implemented*/

		}
		return currTime() - startTime;

	}

	/*
	 * Test the performance of the substr() function of our strType
	 */
	long substr(long numRuns, int strType) {

		long startTime = currTime();
		String longStr = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz";
		String subStr = "";
		int pos;
		Random rand = new Random(); //random generator

		switch(strType) {

		case JAVA_LANG_STRING:

			for(int i = 0; i < numRuns; ++i){
				for(int j = 0; j < ALOT; ++j){
					subStr = longStr.substring(pos = rand.nextInt(longStr.length()-1), pos + rand.nextInt((longStr.length()-1-pos)));
				}
			}
			System.err.println("subStr: " + subStr); //force the compiler to actually do the operations because subStr is used!

			/*case STD_STRING: TODO Will have to wait until STL is implemented*/

		}
		return currTime() - startTime;

	}

	/*
	 * Test the performance of string comparisons on our strType
	 */
	long compare(long numRuns, int strType, int cmpType) { //TODO change the strings dynamically otherwise the JIT-compiler can see if a string differ/or not and that it never change, (this would actually be possible to foresee for a regular compiler also when the strings are constant

		long startTime = currTime();
		boolean ret;
		String longStr = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz";
		String eq = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz";
		String notEq = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghilj#kmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz";
		String less = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljjkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz"; //2 j:s in a row makes this string lexiographically less than longStr
		String more = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljmkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz"; //a m after the j makes this string lexiographically more than longStr
		int pos;

		switch(strType) {

		case JAVA_LANG_STRING:

			for(int i = 0; i < numRuns; ++i){
				for(int j = 0; j < ALOT; ++j){
					if(cmpType == 0)
						ret = longStr.equals(notEq);
					else
						ret = longStr.equalsIgnoreCase(notEq);
				}
			}
			//			printf("ret: %d", ret); //force the compiler to actually do the operations because subStr is used!

			/*case STD_STRING: TODO Will have to wait until STL is implemented*/

		}
		return currTime() - startTime;

	}


	/*
	 * Wrapper around this platforms syscall so that we can have the same call on different platforms
	 */
	long currTime() {
		return SystemClock.currentThreadTimeMillis();
	}

}

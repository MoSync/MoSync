#include "stropbench.hpp"


extern "C" {

/* Calls a java function to print a message stored in buf */
void pass_jni_msg(char *buf) {
    /* Callback to Java to print buf */
    jstring msg2java = ((*glob_env).NewStringUTF(buf));
    jclass cls = ((*glob_env).GetObjectClass(*glob_obj));
    jmethodID mid = (*glob_env).GetMethodID(cls, "printMsg", "(Ljava/lang/String;)V");
    if(mid == 0) /* Could not find the method */
    	return;
//    (*glob_env)->ExceptionClear();
    (*glob_env).CallVoidMethod(*glob_obj, mid, msg2java);
}

/* entry point callable from the Dalvik VM */
JNIEXPORT jstring JNICALL Java_com_mosync_STROPBenchNativeAndroidActivity_invokeNativeFunction(JNIEnv* env, jobject javaThis) {
	glob_env = env; //save the java environment
	glob_obj = &javaThis; //save the object that called us!
	StropBencher * sb = new StropBencher();
    sb->bench();
    delete sb;
//	return (*env)->NewStringUTF(env, "Done!");
}


StropBencher::StropBencher() {

}

StropBencher::~StropBencher() {

}


/*
 * Do the benchmarks and print the results to the console
 */
void StropBencher::bench() {

	float time;
	int i;
	char buf[120];

	sprintf(buf, "STROPS Benchmark started!");
	pass_jni_msg(buf);

	//double the amount of test runs until the test take more than RUNNING_TIME to perform
	for(i = 1; (time = this->appender(i, MAUTIL_STRING, 0)) < RUNNING_TIME; i*=2);
	sprintf(buf, "appending %d c_str:s to a std::string in ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->appender(i, MAUTIL_STRING, 1)) < RUNNING_TIME; i*=2);
	sprintf(buf, "appending %d std::strings to a std::string in ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->copy(i, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	sprintf(buf, "copying %d std::string to a std::string in ", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->find(i, MAUTIL_STRING, 0)) < RUNNING_TIME; i*=2);
	sprintf(buf, "searching for each character in the alphabet %d times using find()", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->find(i, MAUTIL_STRING, 1)) < RUNNING_TIME; i*=2);
	sprintf(buf, "searching for each character in the alphabet %d times using find_last_of()", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->find(i, MAUTIL_STRING, 2)) < RUNNING_TIME; i*=2);
	sprintf(buf, "searching for each character in the alphabet %d times using find_first_of()", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->find(i, MAUTIL_STRING, 3)) < RUNNING_TIME; i*=2);
	sprintf(buf, "searching for each character in the alphabet %d times using find_first_not_of()", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->substr(i, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	sprintf(buf, "substring a std::string and store in another std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->insert(i, MAUTIL_STRING, 0)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Inserting a c_str in a std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);
	return;

	for(i = 1; (time = this->insert(i, MAUTIL_STRING, 1)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Inserting a std::string in another std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->remove(i, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Removing parts of a std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->resize(i, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Resizing a std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 0)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Testing operator== on std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 1)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Testing operator!= on std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 2)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Testing operator<= on std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 3)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Testing operator>= on std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 4)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Testing operator< on std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 5)) < RUNNING_TIME; i*=2);
	sprintf(buf, "Testing operator> on std::string %d times", ALOT*i);
	pass_jni_msg(buf);
	sprintf(buf, "Time: %1.2f seconds, %1.2f KSTROPS.\n", time, (float) (0.001f*ALOT*i)/(float) time);
	pass_jni_msg(buf);

	//freeze until 0 is pressed
	//FREEZE;
}




/*
 * Bench string appending
 */
float StropBencher::appender(int numRuns, int strType, int func) {

	float startTime = currTime();
    std::string str("Ima String!");
	std::string appStr("Append me!");
	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				if(func == 0)
					appStr.append("junk", 4);
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
 * Bench string insertion
 * @param insType, which insert()-func to call
 */
float StropBencher::insert(int numRuns, int strType, int insType) {

	std::string str("I am a string!");
	std::string inStr("I am a string to be inserted into other strings!");

	float startTime = currTime();
	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				if(insType == 0)
					str.insert(4, "junk");
				else
					str.insert(2, inStr); //this is a lot slower about 5 times in MoRE
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
float StropBencher::copy(int numRuns, int strType) {

	float startTime = currTime();
	std::string tmp;
	std::string cpyStr = std::string("junk");
	std::string cpyStr2 = std::string("more junk");
	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				tmp = cpyStr2;
				cpyStr2 = cpyStr;
				cpyStr = tmp;
			}
		}
		printf("cpyStr = %s", cpyStr.c_str());
		

		/*case STD_STRING: TODO Will have to wait until STL is implemented*/

	}
    return currTime() - startTime;

}

/*
 * Benchmark the find functions family
 * @param func tells which find-function to bench
 */
float StropBencher::find(int numRuns, int strType, int func) {

	float startTime = currTime();
	std::string alphabet("abcdefghiljkmnopqrstuvwxyz");
	const char * c_str = "a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0k\0l\0m\0n\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z\0";
	std::string largeAlphabet("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	int res; //result of find (position where char was found or -1 on no match)

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				for(char k = 0; k < alphabet.length(); ++k){
					if(func == 0){
						res = alphabet.find(c_str+2*k, 0);
						if(res == -1)
							printf("Error in find(), no match but it should be!");
					}else if(func == 1){
						res = largeAlphabet.find_last_of((char) k+97); //ascii val of 'a' is 97
						if(res == -1)
							printf("Error in find_last_of(), no match but it should be!");
					}else if(func == 2){
						res = largeAlphabet.find_first_of((char) k+97, rand()%(largeAlphabet.length()-1)); //randomize starting position
					}else{
						res = largeAlphabet.find_first_not_of((char) k+97, rand()%(largeAlphabet.length()-1)); //randomize starting position
						if(res == -1)
							printf("Error in find_first_not_of(), no match but it should be!");
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
float StropBencher::substr(int numRuns, int strType) {

	float startTime = currTime();
	std::string longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string subStr;
	int pos;

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				subStr = longStr.substr(pos = rand()%(longStr.length()-1), rand()%(longStr.length()-pos));
			}
		}
		printf("subStr: %s", subStr.c_str()); //force the compiler to actually do the operations because subStr is used!
		

		/*case STD_STRING: TODO Will have to wait until STL is implemented*/

	}
    return currTime() - startTime;

}

/*
 * Test the performance of the remove() function of our strType
 */
float StropBencher::remove(int numRuns, int strType) { //TODO remove this, std::string do not have remove()

	float startTime = currTime();
	std::string longStr;
	std::string subStr;
	int pos;

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				longStr = std::string("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
				//longStr.remove(pos = rand()%(longStr.length()-1), rand()%(longStr.length()-pos)); 
			}
		}
		printf("longStr: %s", longStr.c_str()); //force the compiler to actually do the operations because subStr is used!

		/*case STD_STRING: TODO Will have to wait until STL is implemented*/

	}
    return currTime() - startTime;
    
}

/*
 * Test the performance of the resize() function of our strType
 */
float StropBencher::resize(int numRuns, int strType) {

	float startTime = currTime();
	std::string longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string subStr;
	int pos;

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				longStr.resize(rand()%500);
			}
		}
		printf("longStr: %s", longStr.c_str()); //force the compiler to actually do the operations because subStr is used!

		/*case STD_STRING: TODO Will have to wait until STL is implemented*/

	}
    return currTime() - startTime;

}

/*
 * Test the performance of string comparisons on our strType
 */
float StropBencher::compare(int numRuns, int strType, int cmpType) {

	float startTime = currTime();
	bool ret;
	std::string longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string eq("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string notEq("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghilj#kmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string less("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljjkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz"); //2 j:s in a row makes this string lexiographically less than longStr
	std::string more("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljmkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz"); //a m after the j makes this string lexiographically more than longStr
	int pos;

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				if(cmpType == 0)
					ret = longStr == eq;
				else if(cmpType == 1)
					ret = longStr != notEq;
				else if(cmpType == 2)
					ret = longStr <= more;
				else if(cmpType == 3)
					ret = longStr >= less;
				else if(cmpType == 4)
					ret = longStr < less;
				else
					ret = longStr > less;

			}
		}
//		printf("ret: %d", ret); //force the compiler to actually do the operations because subStr is used!

		/*case STD_STRING: TODO Will have to wait until STL is implemented*/

	}
    return currTime() - startTime;

}

/*
 * Wrapper around mosync syscall so that we can have the same call on different platforms
 */
float StropBencher::currTime() {
	return ((float)((float)clock()/(float)CLOCKS_PER_SEC));
    //return maGetMilliSecondCount();
}


}

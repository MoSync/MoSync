/*
 * stropbench.cpp
 *
 * This class do the actual benching
 */

#include "stropbench.h"

StropBencher::StropBencher() {

	this->mMaStr = new MAUtil::String();


}

StropBencher::~StropBencher() {

	delete mMaStr;

}

/*
 * Do the benchmarks and print the results to the console
 */
void StropBencher::bench() {

	int time;
	int i;

	printf("STROPS Benchmark started\n");
	printf("std::string tests:\n");

	/*******************************************
	 * std::string tests					   *
	 *******************************************/

	for(i = 1; (time = this->appender(i, STD_STRING, 0)) < RUNNING_TIME; i*=2);
	printf("append(char*) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->appender(i, STD_STRING, 1)) < RUNNING_TIME; i*=2);
	printf("append(string) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->copy(i, STD_STRING)) < RUNNING_TIME; i*=2);
	printf("copying strings %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->find(i, STD_STRING, 0)) < RUNNING_TIME; i*=2);
	printf("find() %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->find(i, STD_STRING, 1)) < RUNNING_TIME; i*=2);
	printf("findLastOf() %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->find(i, STD_STRING, 2)) < RUNNING_TIME; i*=2);
	printf("findFirstOf() %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->find(i, STD_STRING, 3)) < RUNNING_TIME; i*=2);
	printf("findFirstNotOf() %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->substr(i, STD_STRING)) < RUNNING_TIME; i*=2);
	printf("substring(string) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->insert(i, STD_STRING, 0)) < RUNNING_TIME; i*=2);
	printf("insert(char*) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->insert(i, STD_STRING, 1)) < RUNNING_TIME; i*=2);
	printf("insert(string) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

//	for(i = 1; (time = this->remove(i, STD_STRING)) < RUNNING_TIME; i*=2); NOT IMPLEMENTED IN STD::STRING!
//	printf("Removing parts of a MAUtil::String %d times", ALOT*i);
//	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->resize(i, STD_STRING)) < RUNNING_TIME; i*=2);
	printf("Resizing a string %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, STD_STRING, 0)) < RUNNING_TIME; i*=2);
	printf("operator== %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, STD_STRING, 1)) < RUNNING_TIME; i*=2);
	printf("operator!= %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, STD_STRING, 2)) < RUNNING_TIME; i*=2);
	printf("operator<= %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, STD_STRING, 3)) < RUNNING_TIME; i*=2);
	printf("operator>= %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, STD_STRING, 4)) < RUNNING_TIME; i*=2);
	printf("operator< %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, STD_STRING, 5)) < RUNNING_TIME; i*=2);
	printf("operator> %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	/************************************
	 * MAUtil::String tests				*
	 ************************************/

	printf("MAUtil::String tests:\n");
	//double the amount of test runs until the test take more than RUNNING_TIME to perform
	for(i = 1; (time = this->appender(i, MAUTIL_STRING, 0)) < RUNNING_TIME; i*=2);
	printf("append(char*) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->appender(i, MAUTIL_STRING, 1)) < RUNNING_TIME; i*=2);
	printf("append(string) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->copy(i, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	printf("copying strings %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->find(i, MAUTIL_STRING, 0)) < RUNNING_TIME; i*=2);
	printf("find() %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->find(i, MAUTIL_STRING, 1)) < RUNNING_TIME; i*=2);
	printf("findLastOf() %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->find(i, MAUTIL_STRING, 2)) < RUNNING_TIME; i*=2);
	printf("findFirstOf() %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->find(i, MAUTIL_STRING, 3)) < RUNNING_TIME; i*=2);
	printf("findFirstNotOf() %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->substr(i, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	printf("substring(string) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->insert(i, MAUTIL_STRING, 0)) < RUNNING_TIME; i*=2);
	printf("insert(char*) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->insert(i, MAUTIL_STRING, 1)) < RUNNING_TIME; i*=2);
	printf("insert(string) %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->remove(i, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	printf("remove() parts of a string %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->resize(i, MAUTIL_STRING)) < RUNNING_TIME; i*=2);
	printf("Resizing a string %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 0)) < RUNNING_TIME; i*=2);
	printf("operator== %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 1)) < RUNNING_TIME; i*=2);
	printf("operator!= %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 2)) < RUNNING_TIME; i*=2);
	printf("operator<= %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 3)) < RUNNING_TIME; i*=2);
	printf("operator>= %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 4)) < RUNNING_TIME; i*=2);
	printf("operator< %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	for(i = 1; (time = this->compare(i, MAUTIL_STRING, 5)) < RUNNING_TIME; i*=2);
	printf("operator> %dK times: ", ALOT*i/1000);
	printf("%1.2f KSTROPS\n", (float) (ALOT*i)/(float) time);

	//freeze until 0 is pressed
	FREEZE;
}


/*
 * Bench string appending
 */
int StropBencher::appender(int numRuns, int strType, int func) {

	int startTime = currTime();
	MAUtil::String str("Ima String!");
	MAUtil::String appStr("Append me!");
	std::string _str("Ima String!");
	std::string _appStr("Append me!");
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
		return currTime() - startTime;

	case STD_STRING: //TODO Will have to wait until STL is implemented

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				if(func == 0)
					_appStr.append("junk");
				else if(func == 1)
					_appStr += _str;
			}
		}
		return currTime() - startTime;

	}

}

/*
 * Bench string insertion
 * @param insType, which insert()-func to call
 */
int StropBencher::insert(int numRuns, int strType, int insType) {

	MAUtil::String str("I am a string!");
	MAUtil::String inStr("I am a string to be inserted into other strings!");
	std::string _str("I am a string!");
	std::string _inStr("I am a string to be inserted into other strings!");

	int startTime = currTime();
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
		return currTime() - startTime;

	case STD_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				if(insType == 0)
					_str.insert(4, "junk");
				else
					_str.insert(2, _inStr); //this is a lot slower about 5 times in MoRE
			}
		}
		return currTime() - startTime;

	}

}

/*
 * copy 2 strings back and forth
 */
int StropBencher::copy(int numRuns, int strType) {

	int startTime = currTime();
	MAUtil::String tmp;
	MAUtil::String cpyStr = MAUtil::String("junk");
	MAUtil::String cpyStr2 = MAUtil::String("more junk");
	std::string _tmp;
	std::string _cpyStr = std::string("junk");
	std::string _cpyStr2 = std::string("more junk");

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				tmp = cpyStr2;
				cpyStr2 = cpyStr;
				cpyStr = tmp;
			}
		}
		//printf("cpyStr = %s", cpyStr.c_str());
		return currTime() - startTime;

	case STD_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				_tmp = _cpyStr2;
				_cpyStr2 = _cpyStr;
				_cpyStr = _tmp;
			}
		}
		//printf("cpyStr = %s", cpyStr.c_str());
		return currTime() - startTime;

	}

}

/*
 * Benchmark the find functions family
 * @param func tells which find-function to bench
 */
int StropBencher::find(int numRuns, int strType, int func) {

	int startTime = currTime();
	MAUtil::String alphabet("abcdefghiljkmnopqrstuvwxyz");
	const char * c_str = "a\0b\0c\0d\0e\0f\0g\0h\0i\0j\0k\0l\0m\0n\0o\0p\0q\0r\0s\0t\0u\0v\0w\0x\0y\0z\0";
	MAUtil::String largeAlphabet("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string _alphabet("abcdefghiljkmnopqrstuvwxyz");
	std::string _largeAlphabet("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	int res; //result of find (position where char was found or -1 on no match)

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				for(char k = 0; k < alphabet.length(); ++k){
					if(func == 0){
						res = alphabet.find(c_str+2*k, 0); //a little hack to make this compile (char)k+97 will not work here
						if(res == -1)
							printf("Error in find(), no match but it should be!");
					}else if(func == 1){
						res = largeAlphabet.findLastOf((char) k+97); //ascii val of 'a' is 97
						if(res == -1)
							printf("Error in findLastOf(), no match but it should be!");
					}else if(func == 2){
						res = largeAlphabet.findFirstOf((char) k+97, rand()%(largeAlphabet.length()-1)); //randomize starting position
					}else{
						res = largeAlphabet.findFirstNotOf((char) k+97, rand()%(largeAlphabet.length()-1)); //randomize starting position
						if(res == -1)
							printf("Error in findFirstNotOf(), no match but it should be!");
					}
				}
			}
		}
		return currTime() - startTime;

	case STD_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				for(char k = 0; k < _alphabet.length(); ++k){
					if(func == 0){
						res = _alphabet.find(c_str+2*k, 0); //a little hack to make this compile (char)k+97 will not work here
						if(res == -1)
							printf("Error in find(), no match but it should be!");
					}else if(func == 1){
						res = _largeAlphabet.find_last_of((char) k+97); //ascii val of 'a' is 97
						if(res == -1)
							printf("Error in findLastOf(), no match but it should be!");
					}else if(func == 2){
						res = _largeAlphabet.find_first_of((char) k+97, rand()%(_largeAlphabet.length()-1)); //randomize starting position
					}else{
						res = _largeAlphabet.find_first_not_of((char) k+97, rand()%(_largeAlphabet.length()-1)); //randomize starting position
						if(res == -1)
							printf("Error in findFirstNotOf(), no match but it should be!");
					}
				}
			}
		}
		return currTime() - startTime;

	}

}

/*
 * Test the performance of the substr() function of our strType
 */
int StropBencher::substr(int numRuns, int strType) {

	int startTime = currTime();
	MAUtil::String longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	MAUtil::String subStr;
	std::string _longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string _subStr;
	int pos;

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				subStr = longStr.substr(pos = rand()%(longStr.length()-1), rand()%(longStr.length()-pos));
			}
		}
		//printf("subStr: %s", subStr.c_str()); //force the compiler to actually do the operations because subStr is used!
		return currTime() - startTime;

	case STD_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				_subStr = _longStr.substr(pos = rand()%(_longStr.length()-1), rand()%(_longStr.length()-pos));
			}
		}
		//printf("subStr: %s", subStr.c_str()); //force the compiler to actually do the operations because subStr is used!
		return currTime() - startTime;

	}

}

/*
 * Test the performance of the remove() function of our strType
 */
int StropBencher::remove(int numRuns, int strType) {

	int startTime = currTime();
	MAUtil::String longStr;
	MAUtil::String subStr;
	std::string _longStr;
	std::string _subStr;
	int pos;

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				longStr = MAUtil::String("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
				longStr.remove(pos = rand()%(longStr.length()-1), rand()%(longStr.length()-pos)); //randomize starting position from 0 -> strlen-1 and length from 0->strlen-startpos
			}
		}
		//printf("longStr: %s", longStr.c_str()); //force the compiler to actually do the operations because subStr is used!
		return currTime() - startTime;

	case STD_STRING:

		/* std::string has no remove member */
		//printf("longStr: %s", longStr.c_str()); //force the compiler to actually do the operations because subStr is used!
		return -1;

	}

}

/*
 * Test the performance of the resize() function of our strType
 */
int StropBencher::resize(int numRuns, int strType) {

	int startTime = currTime();
	MAUtil::String longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	MAUtil::String subStr;
	std::string _longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string _subStr;
	int pos;

	switch(strType) {

	case MAUTIL_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				longStr.resize(rand()%500);
			}
		}
		//printf("longStr: %s", longStr.c_str()); //force the compiler to actually do the operations because subStr is used!
		return currTime() - startTime;

	case STD_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				_longStr.resize(rand()%500);
			}
		}
		//printf("longStr: %s", longStr.c_str()); //force the compiler to actually do the operations because subStr is used!
		return currTime() - startTime;

	}

}

/*
 * Test the performance of string comparisons on our strType
 */
int StropBencher::compare(int numRuns, int strType, int cmpType) {

	int startTime = currTime();
	bool ret;
	MAUtil::String longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	MAUtil::String eq("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	MAUtil::String notEq("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghilj#kmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	MAUtil::String less("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljjkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz"); //2 j:s in a row makes this string lexiographically less than longStr
	MAUtil::String more("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljmkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz"); //a m after the j makes this string lexiographically more than longStr
	std::string _longStr("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string _eq("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string _notEq("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghilj#kmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz");
	std::string _less("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljjkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz"); //2 j:s in a row makes this string lexiographically less than longStr
	std::string _more("abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljmkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz"); //a m after the j makes this string lexiographically more than longStr
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
		return currTime() - startTime;

	case STD_STRING:

		for(int i = 0; i < numRuns; ++i){
			for(int j = 0; j < ALOT; ++j){
				if(cmpType == 0)
					ret = _longStr == _eq;
				else if(cmpType == 1)
					ret = _longStr != _notEq;
				else if(cmpType == 2)
					ret = _longStr <= _more;
				else if(cmpType == 3)
					ret = _longStr >= _less;
				else if(cmpType == 4)
					ret = _longStr < _less;
				else
					ret = longStr > less;

			}
		}
		//		printf("ret: %d", ret); //force the compiler to actually do the operations because subStr is used!
		return currTime() - startTime;

	}

}

/*
 * Wrapper around mosync syscall so that we can have the same call on different platforms
 */
int StropBencher::currTime() {
	return maGetMilliSecondCount();
}



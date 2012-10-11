/*
 * benchdb.h
 *
 *  Created on: 3 nov 2011
 *      Author: Alex
 */

#ifndef BENCHDB_H_
#define BENCHDB_H_

#include <MAUtil/Environment.h>
#include <mautil/connection.h>
#include <mastdlib.h>
#include <conprint.h>
#define CONNECTION_BUFFER_SIZE 1024
#define M_URL "http://modev.mine.nu:8070/benchmark/publish_result.php" //Change this to change the target URL of all result uploads

struct BenchResult {

	//hardware/software information
	char * benchmark;
	char * revision;
	char * runtime;
	char * git_hash;
	char * phone;
	char * nativeSdkVer;

	//linpack results
	float mflops;

	//opengles results
	int test1, test2, test3, test4;

	//strops results

	//membench results



};

class BenchDBConnector : private MAUtil::HttpConnectionListener {
public:
	BenchDBConnector(BenchResult& br);

	//HTTP CONNECTION METHODS
	void httpFinished(MAUtil::HttpConnection *conn, int result);
	void connRecvFinished(MAUtil::Connection *conn, int result);
	void connReadFinished(MAUtil::Connection *conn, int result);
	void initiateConnection(const char* url);

private:

	//HTTP CONNECTION VARS
	char mBuffer[CONNECTION_BUFFER_SIZE];
	MAUtil::HttpConnection mHttp;
	bool mIsConnected;
};


//#ifdef __cplusplus
//extern "C" {
//#endif
//
//int publish_linpack_result(char * revision, char * runtime, char * git_hash, char * phone,
//		char * native_sdk_ver, float mflops);
//
//int publish_opengl_result(char * revision, char * runtime, char * git_hash, char * phone,
//		char * native_sdk_ver, int test1, int test2, int test3, int test4);
//
//int publish_membench_result(char * revision, char * runtime, char * git_hash, char * phone,
//		char * native_sdk_ver, float alloc_str_10, float alloc_str_100, float alloc_void_1, float alloc_void_100,
//		float alloc_void_1000, float alloc_dummy, float alloc_dummy_struct, float alloc_dummy_mix,
//		float access_array, float access_vector, float add_vector, float access_dummy, float access_dummy_struct,
//		float access_dummy_mix);
//
//#ifdef __cplusplus
//}
//#endif

#endif /* BENCHDB_H_ */

/*
 * benchdb.h
 *
 *  Created on: 3 nov 2011
 *      Author: Alex
 */

#ifndef BENCHDB_H_
#define BENCHDB_H_

#ifdef __cplusplus
extern "C" {
#endif

int publish_linpack_result(char * url, char * revision, char * runtime, char * git_hash, char * phone,
		char * native_sdk_ver, float mflops);

int publish_opengl_result(char * url, char * revision, char * runtime, char * git_hash, char * phone,
		char * native_sdk_ver, int test1, int test2, int test3, int test4);

int _publish_membench_result(char * url, char * revision, char * runtime, char * git_hash, char * phone,
		char * native_sdk_ver, float alloc_str_10, float alloc_str_100, float alloc_void_1, float alloc_void_100,
		float alloc_void_1000, float alloc_dummy, float alloc_dummy_struct, float alloc_dummy_mix,
		float access_array, float access_vector, float add_vector, float access_dummy, float access_dummy_struct,
		float access_dummy_mix);

#ifdef __cplusplus
}
#endif

#endif /* BENCHDB_H_ */

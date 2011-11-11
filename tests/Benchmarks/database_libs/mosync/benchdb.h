/*
 * benchdb.h
 *
 *  Created on: 3 nov 2011
 *      Author: Alex
 */

#ifndef BENCHDB_H_
#define BENCHDB_H_


int publish_linpack_result(char * url, char * revision, char * runtime, char * git_hash, char * phone, char * native_sdk_ver, float mflops);


#endif /* BENCHDB_H_ */

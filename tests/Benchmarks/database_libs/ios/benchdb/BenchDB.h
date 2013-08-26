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

//
//	BenchDB.h
//	GLBenchIOSNative
//
//	Created by Alex on 1/5/12.
//

#import <Foundation/Foundation.h>
#import <Foundation/NSURLRequest.h>
#import <Foundation/NSURLConnection.h>
#import <UIKit/UIkit.h>

#define M_URL "http://modev.mine.nu:8070/benchmark/publish_result.php?" //Change this to change the target URL of all result uploads

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
	float alloc_str_10, alloc_str_100, alloc_void_1, alloc_void_100, alloc_void_1000, alloc_dummy, alloc_dummy_struct, alloc_dummy_mix,
	access_array, access_dummy, access_vector, add_vector, access_dummy_struct, access_dummy_mix;

};


@interface BenchDBConnector:NSObject {
	struct BenchResult mBr;
	NSMutableData* mReceivedData;
}
//Constructor
- (int) submit:(struct BenchResult) br;


@end

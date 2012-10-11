//
//  BenchDB.h
//  GLBenchIOSNative
//
//  Created by Alex on 1/5/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
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
    


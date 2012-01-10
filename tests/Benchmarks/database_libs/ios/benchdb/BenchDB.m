//
//  BenchDB.m
//  GLBenchIOSNative
//
//  Created by Alex on 1/5/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "BenchDB.h"


@implementation BenchDBConnector

- (int) submit:(struct BenchResult)br {
    
    br.revision = "0";
    br.runtime = "iOS";
    br.git_hash = "1";
    br.phone = "iOSSimulator";
    br.nativeSdkVer = "xcode123";
    
    //build url-string
    NSString *url = [NSString stringWithCString:M_URL encoding:NSUTF8StringEncoding];
    url = [url stringByAppendingFormat:@"benchmark=%s", br.benchmark]; 
    url = [url stringByAppendingFormat:@"&revision=%s", br.revision];
    url = [url stringByAppendingFormat:@"&runtime=%s", br.runtime];
    url = [url stringByAppendingFormat:@"&git_hash=%s", br.git_hash];
    url = [url stringByAppendingFormat:@"&phone=%s", br.phone];
    url = [url stringByAppendingFormat:@"&native_sdk_ver=%s", br.nativeSdkVer];
    
    if(strcmp(br.benchmark, "opengl") == 0){ //opengl benchmark result
        url = [url stringByAppendingFormat:@"&test1=%d", br.test1];
        url = [url stringByAppendingFormat:@"&test2=%d", br.test2];
        url = [url stringByAppendingFormat:@"&test3=%d", br.test3];
        url = [url stringByAppendingFormat:@"&test4=%d", br.test4];
    }else if(strcmp(br.benchmark, "linpack") == 0){ //linpack benchmark result
        url = [url stringByAppendingFormat:@"&mflops=%.3f", br.mflops];
    } //other benchmarks here
    
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]
                               cachePolicy:NSURLRequestUseProtocolCachePolicy
                           timeoutInterval:60.0];
    
    //TODO identify benchmark type, build url-string, make library :)
    
    NSURLConnection *connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
    if (connection) {
        // Create the NSMutableData to hold the received data.
        // receivedData is an instance variable declared elsewhere.
        mReceivedData = [[NSMutableData data] retain];
    } else {
        // Inform the user that the connection failed.
    }
    
    
    
    return 0; //TODO return the result code of the URLConnection object
}

@end

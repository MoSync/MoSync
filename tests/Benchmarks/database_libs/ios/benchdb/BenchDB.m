//
//  BenchDB.m
//  GLBenchIOSNative
//
//  Created by Alex on 1/5/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#import "BenchDB.h"


@implementation BenchDBConnector

- (id) init{
    if((self = [super init]))
    {

    }
    return self;
}
/* Submits the benchmark results to the benchmark server using HTTP GET. Returns 0 if the GET request succeeds and -1 if it fails. */
- (int) submit:(struct BenchResult)br {
    
    br.revision = "0";
    br.runtime = [[[[UIDevice currentDevice] systemName] stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding ] cStringUsingEncoding:NSUTF8StringEncoding];
    br.git_hash = "0";
    br.phone = [[[[UIDevice currentDevice] name] stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding ] cStringUsingEncoding:NSUTF8StringEncoding];
;//[[[UIDevice currentDevice] model] cStringUsingEncoding:NSUTF8StringEncoding];
    printf("__VERSION__ %s\n", __VERSION__);
    br.nativeSdkVer = [[[NSString stringWithUTF8String: __VERSION__] stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding ] cStringUsingEncoding:NSUTF8StringEncoding];
    
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
    }else if(strcmp(br.benchmark, "membench") == 0){
        url = [url stringByAppendingFormat:@"&alloc_str_10=%.3f", br.alloc_str_10];
        url = [url stringByAppendingFormat:@"&alloc_str_100=%.3f", br.alloc_str_100];
        url = [url stringByAppendingFormat:@"&alloc_void_1=%.3f", br.alloc_void_1];
        url = [url stringByAppendingFormat:@"&alloc_void_100=%.3f", br.alloc_void_100];
        url = [url stringByAppendingFormat:@"&alloc_void_1000=%.3f", br.alloc_void_1000];
        url = [url stringByAppendingFormat:@"&alloc_dummy=%.3f", br.alloc_dummy];
        url = [url stringByAppendingFormat:@"&alloc_dummy_struct=%.3f", br.alloc_dummy_struct];
        url = [url stringByAppendingFormat:@"&alloc_dummy_mix=%.3f", br.alloc_dummy_mix];
        url = [url stringByAppendingFormat:@"&access_array=%.3f", br.access_array];
        url = [url stringByAppendingFormat:@"&access_vector=%.3f", br.access_vector];
        url = [url stringByAppendingFormat:@"&add_vector=%.3f", br.add_vector];
        url = [url stringByAppendingFormat:@"&access_dummy=%.3f", br.access_dummy];
        url = [url stringByAppendingFormat:@"&access_dummy_struct=%.3f", br.access_dummy_struct];
        url = [url stringByAppendingFormat:@"&access_dummy_mix=%.3f", br.access_dummy_mix];
    }
    
    NSLog(@"benchdblib: url: %@\n", url);
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]
                               cachePolicy:NSURLRequestUseProtocolCachePolicy
                           timeoutInterval:60.0];
    NSHTTPURLResponse *response = nil;
    NSError *error;
    NSData *responseData;
    
    //NSURLConnection *connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
    responseData = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
    if (response != nil) {
        // Create the NSMutableData to hold the received data.
        // receivedData is an instance variable declared elsewhere.
        //mReceivedData = [[NSMutableData data] retain];
        NSLog(@"iOS Benchmark DONE!");
        exit(0); //connection went fine
        //return 0; //connection went fine
    } else {
        // Inform the user that the connection failed.
        NSLog(@"connection failed with HTTP status code: %d", [response statusCode]);
        NSString *responseString = [[NSString alloc] initWithData:responseData encoding:NSUTF8StringEncoding];
        NSLog(@"response: %@", responseString);
        return -1;
    }
    
}



@end

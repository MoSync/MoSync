package com.mosync;
import java.net.URI;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

/*
 * Class for publishing benchmark results to the benchmark database
 * import com.mosync.BenchDB; to use it! 
 */
public class BenchDB {
	
	HttpClient client;
	HttpGet request;
	
	public BenchDB() {
		client = new DefaultHttpClient();
		request = new HttpGet();
	}
	
	//publish linpack result
	public void publishLinpackResult(double mflops) throws Exception { //10.0.2.2 is the address of the host machine, i.e the host that runs the emulator
		request.setURI(new URI("http://modev.mine.nu:8070/benchmark/publish_result.php?benchmark=linpack&revision=1337&runtime=Android%20Dalvik&git_hash=981723987&phone=HTC%20Wildfire&mflops=" + mflops + "&test=0&native_sdk_ver=2"));
		HttpResponse response = client.execute(request);
		int i = 1;
	}
	
	//publish OpenGL result
	public void publishOpenglResult() throws Exception {
		
	}
	
	//publish membench result
	public void publishMembenchResult() throws Exception {
		
	}
	
	public void publishStropbenchResult() throws Exception {
		
	}
}
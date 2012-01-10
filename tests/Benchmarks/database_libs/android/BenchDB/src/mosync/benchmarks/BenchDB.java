package mosync.benchmarks;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.os.Build;

/*
 * Class for publishing benchmark results to the benchmark database
 * import com.mosync.BenchDB; to use it! 
 */
public class BenchDB {

	HttpClient client;
	HttpGet request;

	static final String URL = "http://modev.mine.nu:8070/benchmark/publish_result.php?"; //should point to the result upload script

	public BenchDB(BenchResult br) {
		client = new DefaultHttpClient();
		request = new HttpGet();
		
		br.git_hash = Build.VERSION.INCREMENTAL;
		br.nativeSdkVer = String.valueOf(Build.VERSION.SDK_INT); //get the native SDK version number
		br.phone = Build.PRODUCT;
		br.revision = ""; //mosync revision not applicable here since this is the Android Dalvik/NDK library

		try {
			request.setURI(new URI(URL + "benchmark=linpack&revision=" + br.revision + "&runtime=Android%20Dalvik&git_hash=" + br.git_hash + 
					"&phone=" + br.phone + "&mflops="  + br.mflops + "&native_sdk_ver=" + br.nativeSdkVer));
		} catch (URISyntaxException e) {
			System.err.println("Malformed URL in BenchDB() constructor in BenchDB.java!");
			e.printStackTrace();
		}
		HttpResponse response;
		try {
			response = client.execute(request);
			System.out.println(response);
		} catch (ClientProtocolException e) {
			System.err.println("Error during the http-request phase");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("Error during the http-response phase");
			e.printStackTrace();
		}
		
	}

	//publish linpack result (need to get device info here
	public void publishLinpackResult(String revision, String git_hash, String native_sdk_ver, double mflops) throws Exception { //10.0.2.2 is the address of the host machine, i.e the host that runs the emulator
		request.setURI(new URI(URL + "benchmark=linpack&revision=" + revision + "&runtime=Android%20Dalvik&git_hash=" + git_hash + 
				"&phone=" + Build.PRODUCT + "&mflops="  + mflops + "&native_sdk_ver=" + native_sdk_ver));
		HttpResponse response = client.execute(request);
		System.out.println(response);
		//		int i = 1;
	}

	//publish OpenGL result TODO all test params
	public void publishOpenglResult(String revision, String git_hash, String native_sdk_ver, int test1, int test2, int test3, int test4) throws Exception {
		request.setURI(new URI(URL + "benchmark=opengl&revision=" + revision + "&runtime=Android%20Dalvik&git_hash=" + git_hash + 
				"&phone=AndroidTestDevice" + "&test1="  + test1 + "&test2=" + test2 + "&test3=" + test3 + "&test4=" + test4 + "&native_sdk_ver=" + native_sdk_ver));
		HttpResponse response = client.execute(request);
		System.out.println(response);
	}

	//publish membench result
	public void publishMembenchResult(String revision, String git_hash, String native_sdk_ver) throws Exception {

	}

	public void publishStropbenchResult() throws Exception {

	}
}
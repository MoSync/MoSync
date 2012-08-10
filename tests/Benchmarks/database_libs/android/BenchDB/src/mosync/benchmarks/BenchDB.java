package mosync.benchmarks;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.BasicResponseHandler;
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
			if(br.benchmark.equals("linpack")){
				request.setURI(new URI(URL + "benchmark=" + br.benchmark + "&revision=" + br.revision + "&runtime=" + br.runtime + "&git_hash=" + br.git_hash +
				"&phone=" + br.phone + "&mflops="  + br.mflops + "&native_sdk_ver=" + br.nativeSdkVer));
			}else if(br.benchmark.equals("opengl")){
				request.setURI(new URI(URL + "benchmark=" + br.benchmark + "&revision=" + br.revision + "&runtime=Android%20Dalvik&git_hash=" + br.git_hash +
				"&phone=" + br.phone + "&test1="  + br.test1 + "&test2="  + br.test2 + "&test3="  + br.test3 + "&test4="  + br.test4 + "&native_sdk_ver=" + br.nativeSdkVer));
			}else if(br.benchmark.equals("membench")){
				request.setURI(new URI(URL + "benchmark=" + br.benchmark + "&runtime=Android%20NDK&git_hash=" + br.git_hash +
				"&phone=" + br.phone + "&alloc_str_10="  + br.str_alloc_10 + "&alloc_str_100=" + br.str_alloc_100 + "&alloc_void_1=" + br.alloc_void_1 +
				"&alloc_void_100=" + br.alloc_void_100 + "&alloc_void_1000=" + br.alloc_void_1000 + "&alloc_dummy=" + br.alloc_dummy + "&alloc_dummy_struct=" +
				br.alloc_dummy_struct + "&alloc_dummy_mix=" + br.alloc_dummy_mix + "&access_array=" + br.access_array + "&access_vector=" + br.access_vector +
				"&add_vector=" + br.add_vector + "&access_dummy=" + br.access_dummy + "&access_dummy_struct=" + br.access_dummy_struct + "&access_dummy_mix=" +
				br.access_dummy_mix + "&native_sdk_ver=" + br.nativeSdkVer));
			}
		} catch (URISyntaxException e) {
			System.err.println("Malformed URL in BenchDB() constructor in BenchDB.java!");
			e.printStackTrace();
		}
		String responseBody = null;
		try {
			ResponseHandler<String> responseHandler=new BasicResponseHandler();
	        responseBody = client.execute(request, responseHandler);
		} catch (ClientProtocolException e) {
			System.err.println("Error during the http-request phase");
			e.printStackTrace();
		} catch (IOException e) {
			System.err.println("Error during the http-response phase");
			System.err.println(e);
		}
		System.err.println("URI: " + request.getURI());
		System.err.println("Response: " + responseBody);
		System.err.println("Android benchmark DONE!\n");
	}

	/* these functions are not used */
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
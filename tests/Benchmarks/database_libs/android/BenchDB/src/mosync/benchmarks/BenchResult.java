package mosync.benchmarks;

public class BenchResult {

	public BenchResult() {

	}

	//hardware/software information
	public String benchmark;
	public String revision;
	public String runtime;
	public String git_hash;
	public String phone;
	public String nativeSdkVer;

	//linpack results
	public float mflops;

	//opengles results
	public int test1, test2, test3, test4;

	//strops results

	//membench results
	public float str_alloc_10, str_alloc_100, alloc_void_1, alloc_void_100, alloc_void_1000, alloc_dummy, alloc_dummy_struct, alloc_dummy_mix,
	access_array, access_dummy, access_vector, add_vector, access_dummy_struct, access_dummy_mix;


}

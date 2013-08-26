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

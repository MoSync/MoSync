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

class BtDevice {
	public String address;
	public String name;
}
class BtService {
	public String address;
	public String name;
	public int port;
	public Vector uuids;
}
DiscoveryAgent mBtDA;
Vector mBtQue = new Vector();
int mBtNextIndex;
boolean mBtDiscoveryInProgress = false;
boolean mBtNames;
int mBtId = -1;

class MyRemoteDevice extends RemoteDevice {
	MyRemoteDevice(final String address) {
		super(address);
	}
}

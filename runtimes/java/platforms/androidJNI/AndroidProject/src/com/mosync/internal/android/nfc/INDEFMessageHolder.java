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

package com.mosync.internal.android.nfc;

import java.io.IOException;

public interface INDEFMessageHolder extends INDEFMessageWritable {

	/**
	 * Sends a request to the tag to get the current
	 * NDEF message on it.
	 * @throws IOException
	 */
	public void requestNDEFMessage(ResourcePool pool) throws IOException;

	/**
	 * Returns a cached ndef message of this holder.
	 * The holder is responsible for destroying it.
	 * @return
	 */
	public NDEFMessage getNDEFMessage();

}

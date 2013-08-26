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

package com.mosync.api;

import com.mosync.internal.android.extensions.MoSyncExtensionLoader;
import com.mosync.internal.android.extensions.StringType;
import com.mosync.internal.android.extensions.TypeDescriptor;



public abstract class Struct implements Marshaller {

	protected static final Marshaller _m(String extension, String name, int ptrDepth) {
		// Special case: String vs CString
		boolean out = StringType.isStringType(name);
		TypeDescriptor desc = MoSyncExtensionLoader.getDefault().getModule(extension).getTypeDescriptor(name, ptrDepth, out);
		return desc;
	}

	public abstract int size();

}

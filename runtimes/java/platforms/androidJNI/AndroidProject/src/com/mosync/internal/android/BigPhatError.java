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

package com.mosync.internal.android;

import android.util.Log;

/**
 * This class represents an irrecoverable error.
 *
 * TODO: Review how this class is used. Either catch it,
 * or replace with a panic call.
 */
@SuppressWarnings("serial")
public class BigPhatError extends Error
{
	public BigPhatError(String s) 
	{ 
		super(s);
		Log.e("@@@ MoSync", "BigPhatError created:" + s);
		new Exception("Stack Trace").printStackTrace();
	}
	
	public BigPhatError() 
	{
		Log.e("@@@ MoSync", "BigPhatError created");
		new Exception("Stack Trace").printStackTrace();
	}
}

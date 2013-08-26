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

package com.mosync.nativeui.util.properties;

/**
 * Exception that is thrown when a system call was unsuccessful,
 * because it is not compatible with the current framework version.
 *
 * @author emma tresanszki
 */
public class FeatureNotAvailableException extends RuntimeException
{
	private static final long serialVersionUID = 1L;

	public FeatureNotAvailableException(String feature)
	{
		super( "The system call: '" + feature +
			   "' is not suported for this platform. '");
	}
}
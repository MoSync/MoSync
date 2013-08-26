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

//printf line with logging
__inline int printflnl(const char* fmt, ...) {
	va_list args;
	char buf[2048];
	int len;

	buf[0] = 0;

	va_start(args, fmt);
	len = vsprintf(buf, fmt, args);
	va_end(args);

	buf[len++] = '\n';
	buf[len] = 0;
	maWriteLog(buf, len);
	PrintConsole(buf);
	return len;
}

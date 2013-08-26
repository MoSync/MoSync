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


//errors are negative. mere problems are positive. success is zero.
typedef enum _XRESULT {
  XR_OK=0, XR_PARSE_ERROR=-1, XR_TAG_NOT_FOUND=1, XR_BUFFER_OVERFLOW=-2,
  XR_COMM_ERROR=-3, XR_INTERNAL_ERROR=-4
} XRES;

#define XISERROR(a) ((a) < 0)
#define XISPROBLEM(a) ((a) > 0)
#define XISOK(a) (!(a))

#define PRINTLINE printiln("line %i", __LINE__)
#define TESTXML(a) { XRES temp = (a); if(XISPROBLEM(temp)) { return temp; }\
  else if(XISERROR(temp)) { PRINTLINE; return temp; } }
#define TX TESTXML
#define TXFAIL(a) { PRINTLINE; return (a); }

XRES XMLInit(Handle httpSocket);

#define MAXLEVEL 8
#define MAXTAGSIZE 32 //including terminator

//will skip all other tags.
//may return XR_TAG_NOT_FOUND if a previously started tag ends. if so, don't call EndTag() for it.
XRES XMLStartTag(const char* expectedTag); 

XRES XMLEndTag();  //every successfully started tag must be ended

/*//*pPtr is not null-terminated, but '<'-terminated.
//uses the most recently started tag.
XRES XMLGetTagContent(const char** pPtr);

//pEnd is a pointer to the first char following the content, a '<'.
int XMLGetTagContentEnd(const char** pPtr, const char** pEnd);*/

//bufsize includes the terminating zero.
//size is strlen(buffer) on success, not modified on failure.
XRES XMLReadTagContent(char* buffer, int bufsize, int* size);

XRES XMLFinish();  //will return error if there are open tags

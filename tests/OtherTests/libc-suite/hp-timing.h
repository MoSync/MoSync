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

#include <stdint.h>

typedef int hp_timing_t;

#define HP_TIMING_AVAIL 0

#define HP_TIMING_DIFF_INIT()
#define HP_TIMING_NOW(a)
#define HP_TIMING_DIFF(a,b,c)

static impl_t** __last_impl_p = (impl_t**)&__stop_impls;

#undef IMPL
#define IMPL(name, test) \
  static impl_t tst_ ## name							\
    = { #name, (void (*) (void))name, test }; \
	void initImpl##name() __attribute__ ((constructor)); \
	void initImpl##name() { \
	**__last_impl_p = tst_ ## name;	(*__last_impl_p)++; }

#define MIN(a,b) ((a) < (b) ? (a) : (b))

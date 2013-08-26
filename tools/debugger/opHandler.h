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

#ifndef OPHANDLER_H
#define OPHANDLER_H

#include <queue>
#include <stack>

//debug
#include <string>
#ifdef _MSC_VER
#include <windows.h>
#include <Dbghelp.h>
#endif

struct Functor {
	void* f;
	int p;
	bool hasParam;
	std::string name;
	int id;
};
typedef bool (*vfptr)();
typedef bool (*ifptr)(int);

class OpHandler {
public:
	OpHandler() {
#ifdef _MSC_VER
		BOOL res = SymInitialize(GetCurrentProcess(), NULL, true);
		_ASSERT(res);
#endif
	}

	void operator()(vfptr f) {
		Functor fct = { (void*)f, 0, false, "unknown", 0};
		input(fct);
	}

	void operator()(ifptr f, int p) {
		Functor fct = { (void*)f, p, true, "unknown", 0};
		input(fct);
	}

	void done() {
		dump();
		_ASSERT(!curEmpty());
		cur().pop();
		while(level() > 0 && curEmpty()) {
			levelDown();
		}
		if(!curEmpty()) {
			Functor f = cur().front();
			execute(f);
		}
		dump();
	}

	void abort() {
		LOG("Op queue aborted at level %i\n", level());
		while(!mOps.empty()) {
			mOps.pop();
		}
	}
private:
	typedef std::queue<Functor> FunctorQueue;

	void input(Functor f) {
#ifdef _MSC_VER
		char buf[1024];
		SYMBOL_INFO& si(*(SYMBOL_INFO*)buf);
		DWORD64 dis;
		ZeroMemory(&si, sizeof(si));
		si.SizeOfStruct = sizeof(si);
		si.MaxNameLen = sizeof(buf) - sizeof(si);
		BOOL res = SymFromAddr(GetCurrentProcess(), (DWORD64)f.f, &dis, &si);
		int err = GetLastError();
		_ASSERT(res);
		f.name = si.Name;
#endif
		static int sId = 1;
		f.id = sId++;

		dump();
		_ASSERT(mActiveHandler == NULL || mActiveHandler == this);
		mActiveHandler = this;
		if(level() < 0)
			levelUp();
		if(curEmpty()) {
			cur().push(f);
			execute(f);
		} else {
			cur().push(f);
		}
		dump();
	}

	void execute(Functor f) {
#ifdef LOGGING_ENABLED
		LOG("OpQueue: executing %i: %s", f.id, f.name.c_str());
		if(f.hasParam)
			LOG(" 0x%x", f.p);
		LOG("\n");
#endif
		FunctorQueue* orig(&cur());
		int origLevel = level();
		int size = cur().size();
		if(size > 0)
			levelUp();
		bool res;
		if(f.hasParam)
			res = ((ifptr)f.f)(f.p);
		else
			res = ((vfptr)f.f)();
		if(curEmpty() && size > 0)
			levelDown();
		if(res) {
			if(origLevel == level()) {
				done();
			} else {
				orig->pop();
			}
		}
	}

	int level() const { return mOps.size()-1; }
	FunctorQueue& cur() { _ASSERT(level() >= 0); return mOps.top(); }
	const FunctorQueue& cur() const { _ASSERT(level() >= 0); return mOps.top(); }
	bool curEmpty() const {
		if(mOps.empty())
			return true;
		return cur().empty();
	}
	void levelUp() {
		mOps.push(FunctorQueue());
	}
	void levelDown() {
		_ASSERT(level() > 0);
		_ASSERT(curEmpty());
		mOps.pop();
	}

	static void errorHandler() {
		_ASSERT(mActiveHandler != NULL);
		mActiveHandler->abort();
		mActiveHandler = NULL;
	}

	void dump() const {
		//copy the queue, since it can't be read in a non-destructive way.
		std::stack<FunctorQueue> ops = mOps;
		LOG("OpQueue: %" PFZT " levels\n", ops.size());
		while(!ops.empty()) {
			FunctorQueue& fq(ops.top());
			LOG(" %" PFZT " elements:\n", fq.size());
			while(!fq.empty()) {
#ifdef LOGGING_ENABLED
				Functor& f(fq.front());
				LOG("  %i: %s", f.id, f.name.c_str());
				if(f.hasParam)
					LOG(" 0x%x", f.p);
				LOG("\n");
#endif
				fq.pop();
			}
			ops.pop();
		}
	}

	std::stack<FunctorQueue> mOps;

	static OpHandler* mActiveHandler;
};

#endif	//OPHANDLER_H

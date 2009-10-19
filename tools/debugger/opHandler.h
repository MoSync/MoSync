#ifndef OPHANDLER_H
#define OPHANDLER_H

#include <queue>
#include <stack>

//debug
#include <string>
#include <windows.h>
#include <Dbghelp.h>

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
		BOOL res = SymInitialize(GetCurrentProcess(), NULL, true);
		_ASSERT(res);
	}

	void operator()(vfptr f) {
		Functor fct = { f, 0, false };
		input(fct);
	}

	void operator()(ifptr f, int p) {
		Functor fct = { f, p, true };
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
		LOG("OpQueue: executing %i: %s", f.id, f.name.c_str());
		if(f.hasParam)
			LOG(" 0x%x", f.p);
		LOG("\n");

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
		LOG("OpQueue: %i levels\n", ops.size());
		while(!ops.empty()) {
			FunctorQueue& fq(ops.top());
			LOG(" %i elements:\n", fq.size());
			while(!fq.empty()) {
				Functor& f(fq.front());
				LOG("  %i: %s", f.id, f.name.c_str());
				if(f.hasParam)
					LOG(" 0x%x", f.p);
				LOG("\n");
				fq.pop();
			}
			ops.pop();
		}
	}

	std::stack<FunctorQueue> mOps;

	static OpHandler* mActiveHandler;
};

#endif	//OPHANDLER_H

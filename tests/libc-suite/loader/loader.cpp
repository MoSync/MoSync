#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <MAUtil/Connection.h>
#include <MAUtil/PlaceholderPool.h>
#include <conprint.h>
#include <maassert.h>
#include <mastdlib.h>

using namespace MAUtil;

#include "config.h"

#define STORE_BASE_NAME "libc_suite_loader_"
#define TEST(a) do { int _res = (a); if(_res < 0) { printf("Error @ %i: %i\n", __LINE__, _res); FREEZE; } } while(0)

class MyMoblet : public Moblet, HttpConnectionListener {
private:
	// the index of the next test to run, or < 0 if there's no list available.
	int mState;
	HttpConnection mHttp;
	MAHandle mListData;
public:
	MyMoblet() : mHttp(this) {
		printf("libc-suite loader\n");
		printf("%s %s\n", __DATE__, __TIME__);

		loadState();
		// if no state, fetch list
		if(mState < 0) {
			fetchList();
		} else {
			loadList();
			checkResult();	// we may have been reloaded from an exiting test.
			runNextTest();
		}
	}
private:
	void checkResult() {
		MAHandle store = maOpenStore(STORE_BASE_NAME "testing", 0);
		if(store == STERR_NONEXISTENT)
			return;
		TEST(store);
		maCloseStore(store, true);
		
		store = maOpenStore("exit_status", 0);
		if(store == STERR_NONEXISTENT) {
			printf("test crashed.\n");
			FREEZE;
		}
		TEST(store);
		int status;
		MAHandle data = PlaceholderPool::alloc();
		TEST(maReadStore(store, data));
		if(maGetDataSize(data) != sizeof(int)) {
			printf("Broken exit_status.\n");
			FREEZE;
		}
		maCloseStore(store, 0);
		maReadData(data, &status, 0, sizeof(int));
		// for now, break on error.
		printf("status: %i\n", status);
		if(status != 0) {
			FREEZE;
		}
		mState++;
		saveState();
	}
	
	void loadState() {
		mState = -1;
		MAHandle data = PlaceholderPool::alloc();
		MAHandle store = maOpenStore(STORE_BASE_NAME "state", 0);
		if(store == STERR_NONEXISTENT)
			return;
		TEST(store);
		TEST(maReadStore(store, data));
		maCloseStore(store, 0);
		if(maGetDataSize(data) != sizeof(int)) {
			printf("Broken state store.\n");
			FREEZE;
		}
		maReadData(data, &mState, 0, sizeof(int));
		PlaceholderPool::put(data);
	}
	
	void saveState() {
		MAHandle store = maOpenStore(STORE_BASE_NAME "state", MAS_CREATE_IF_NECESSARY);
		TEST(store);
		MAHandle data = PlaceholderPool::alloc();
		TEST(maCreateData(data, sizeof(int)));
		maWriteData(data, &mState, 0, sizeof(int));
		TEST(maWriteStore(store, data));
		PlaceholderPool::put(data);
		maCloseStore(store, 0);
	}
	
	void fetchList() {
		mHttp.setListener(this);
		printf("fetching list...\n");
		TEST(mHttp.connect(LIST_URL));
	}
	
	void httpFinished(HttpConnection* http, int result) {
		printf("htf\n");
	}
	
	void connectFinished(Connection*, int result) {
		TEST(result);
		if(result != 200) {
			printf("%i: Unexpected HTTP response code.\n", result);
			return;
		}
		String lenStr;
		TEST(mHttp.getResponseHeader("content-length", &lenStr));
		int len = atoi(lenStr.c_str());
		printf("list is %i bytes.\n", len);
		mListData = PlaceholderPool::alloc();
		TEST(maCreateData(mListData, len));
		mHttp.readToData(mListData, 0, len);
	}
	
	void connReadFinished(Connection*, int result) {
		mHttp.close();
		TEST(result);
		saveList();
		mState = 0;
		saveState();
		runNextTest();
	}
	
	void saveList() {
		MAHandle store = maOpenStore(STORE_BASE_NAME "list", MAS_CREATE_IF_NECESSARY);
		TEST(store);
		TEST(maWriteStore(store, mListData));
		maCloseStore(store, 0);
	}
	
	void loadList() {
		mListData = PlaceholderPool::alloc();
		MAHandle store = maOpenStore(STORE_BASE_NAME "list", 0);
		TEST(store);
		TEST(maReadStore(store, mListData));
		maCloseStore(store, 0);
	}

	// returns NULL if there are no more tests.
	const char* getNextTest() {
		int size = maGetDataSize(mListData);
		char* listBuf = new char[size+1];
		maReadData(mListData, listBuf, 0, size);
		listBuf[size] = 0;
		char* ptr = listBuf;
		// scan to next test
		int i = 0;
		while(true) {
			char* end = strchr(ptr, '\n');
			if(!end)
				return NULL;
			if(i >= mState) {
				*end = 0;
				break;
			}
			ptr = end + 1;
			i++;
		}
		return ptr;
	}
	
	void runNextTest() {
		const char* nextTestUrl = getNextTest();
		if(!nextTestUrl) {
			printf("All tests completed.\n");
			return;
		}
		printf("fetching test %i from:\n", mState);
		printf("%s\n", nextTestUrl);
		mHttp.setListener(&mTestHandler);
		TEST(mHttp.connect(nextTestUrl));
	}

private:
	class TestHandler : public HttpConnectionListener {
	private:
		MAHandle mData;
		
		void httpFinished(HttpConnection* http, int result) {
			printf("htf\n");
		}
	
		void connectFinished(Connection* conn, int result) {
			HttpConnection* http = (HttpConnection*)conn;
			TEST(result);
			if(result != 200) {
				printf("%i: Unexpected HTTP response code.\n", result);
				return;
			}
			String lenStr;
			TEST(http->getResponseHeader("content-length", &lenStr));
			int len = atoi(lenStr.c_str());
			printf("test is %i bytes.\n", len);
			mData = PlaceholderPool::alloc();
			TEST(maCreateData(mData, len));
			http->readToData(mData, 0, len);
		}

		void connReadFinished(Connection* conn, int result) {
			conn->close();
			TEST(result);
			printf("test received, loading...\n");
			MAHandle store = maOpenStore("exit_status", 0);
			if(store != STERR_NONEXISTENT) {
				TEST(store);
				maCloseStore(store, true);
			}
			store = maOpenStore(STORE_BASE_NAME "testing", MAS_CREATE_IF_NECESSARY);
			TEST(store);
			maCloseStore(store, 0);
			maLoadProgram(mData, true);
			printf("maLoadProgram failed.\n");
		}

	} mTestHandler;
};

int MAMain() {
	MyMoblet m;
	Moblet::run(&m);
	return 0;
}

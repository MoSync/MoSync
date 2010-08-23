#include <ma.h>
#include <maheap.h>
#include <testify/testify.hpp>
#include <IX_WEBVIEW.h>

using namespace Testify;

#define assert TESTIFY_ASSERT

// Seems to be needed, even though not called?
int MAMain()
{
	return 0;
}

// ****** Sunny Tests ****** //

void OpenAndCloseWebView()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

void OpenAndCloseWebViewTwice()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

void SetHTML()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewSetHTML("<html><body>Hello World!</body></html>");
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_PAGE_LOADED == e.type);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

void EvaluateScript()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewEvaluateScript("foo = 42;");
	assert(WEBVIEW_OK == result);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

void CallMoSyncService()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewSetHTML("<html><body>Please Wait...<script>setTimeout(\"document.location = 'mosync://Hello'\", 1000);</script></body></html>");
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_PAGE_LOADED == e.type);

	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_REQUEST == e.type);
	// TODO: assert(0 == strcmp(request, "mosync://Hello"));

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

// ****** Cloudy Tests ****** //

void CloseWebViewThatIsNotOpenShouldFail()
{
	int result;
	result = maWebViewClose();
	assert(WEBVIEW_NOT_OPEN == result);
}

void SetHTMLWithNoOpenWebViewShouldFail()
{
	int result;
	result = maWebViewSetHTML("<html><body>Hello World!</body></html>");
	assert(WEBVIEW_NOT_OPEN == result);
}

void SetHTMLWithNULLValueShouldFail()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewSetHTML(NULL);
	assert(WEBVIEW_ERROR == result);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

void EvaluateScriptWithNoOpenWebViewShouldFail()
{
	int result;
	result = maWebViewEvaluateScript("foo = 42;");
	assert(WEBVIEW_NOT_OPEN == result);
}

void EvaluateScriptWithErrorsShouldFail()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewEvaluateScript("foo = fortytwo;");
	assert(WEBVIEW_ERROR == result);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

void EvaluateScriptWithNULLValueShouldFail()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewEvaluateScript(NULL);
	assert(WEBVIEW_ERROR == result);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

// ****** Funny Tests ****** //

void SayThanks()
{
	MAEvent e;
	int result;

	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_OPENED == e.type);

	result = maWebViewSetHTML("<html><body>Have a wonderful day!<script>setTimeout(\"document.location = 'mosync://Done'\", 2000);</script></body></html>");
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_PAGE_LOADED == e.type);

	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_REQUEST == e.type);
	int size = maWebViewGetRequestSize(e.key);
	assert(size > 0);
	char* request = (char*) malloc(size);
	assert(NULL != request);
	result = maWebViewGetRequest(e.key, request, size);
	assert(result > 0);
	assert(0 == strcmp(request, "mosync://Done"));
	free(request);

	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	while (!maGetEvent(&e));
	assert(EVENT_TYPE_WEBVIEW_CLOSED == e.type);
}

Test *TestSuiteArray[] =
{
    Test::create(bind(OpenAndCloseWebView), "OpenAndCloseWebView"),
    Test::create(bind(OpenAndCloseWebViewTwice), "OpenAndCloseWebViewTwice"),
    Test::create(bind(SetHTML), "SetHTML"),
    Test::create(bind(EvaluateScript), "EvaluateScript"),
    Test::create(bind(CallMoSyncService), "CallMoSyncService"),
    Test::create(bind(CloseWebViewThatIsNotOpenShouldFail), "CloseWebViewThatIsNotOpenShouldFail"),
    Test::create(bind(SetHTMLWithNoOpenWebViewShouldFail), "SetHTMLWithNoOpenWebViewShouldFail"),
    Test::create(bind(SetHTMLWithNULLValueShouldFail), "SetHTMLWithNULLValueShouldFail"),
    Test::create(bind(EvaluateScriptWithNoOpenWebViewShouldFail), "EvaluateScriptWithNoOpenWebViewShouldFail"),
    Test::create(bind(EvaluateScriptWithErrorsShouldFail), "EvaluateScriptWithErrorsShouldFail"),
    Test::create(bind(EvaluateScriptWithNULLValueShouldFail), "EvaluateScriptWithNULLValueShouldFail"),
    Test::create(bind(SayThanks), "SayThanks")
};

TestHook TestSuite(TestSuiteArray, "WebViewTestSuite" );

/*
class MyTestCase : public TestCase
{
public:
    MyTestCase ( void )
    : TestCase( "Mickes Test case")
    {
        addTest( bind( &MyTestCase::myTestMethod, this ), "myTestMethod" );
    }

    void myTestMethod ( void )
    {
		assert(...);
    }
};


Test *TestSuiteArray[] =
{
	// This is how a test with a parameter is created.
    Test::create(bind( myTest1, 10), "myTest1"),
    new MyTestCase()
};

*/

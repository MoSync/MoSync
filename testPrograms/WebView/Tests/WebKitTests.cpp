#include <ma.h>
#include <testify/testify.hpp>
#include <IX_WEBVIEW.h>

using namespace Testify;

#define assert TESTIFY_ASSERT

// Seems to be needed, when though not called?
int MAMain()
{
	return 0;
}

// ****** Sunny Tests ****** //

void OpenAndCloseWebView()
{
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
}

void OpenAndCloseWebViewTwice()
{
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
}

void SetHTML()
{
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewSetHTML("<html><body>Hello World!</body></html>");
	assert(WEBVIEW_OK == result);
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
}

void EvaluateScript()
{
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewEvaluateScript("foo = 42;");
	assert(WEBVIEW_OK == result);
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
}

void CallMoSyncService()
{
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewSetHTML("<html><body>Please Wait...<script>setTimeout(\"document.location = 'mosync://Hello'\", 1000);</script></body></html>");
	assert(WEBVIEW_OK == result);
	MAEvent e;
	while (!maGetEvent(&e));
	TESTIFY_ASSERT(EVENT_TYPE_WEBVIEW_SERVICE_REQUEST == e.type);
	assert(0 == strcmp(e.serviceRequest, "mosync://Hello"));
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
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
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewSetHTML(NULL);
	assert(WEBVIEW_ERROR == result);
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
}

void EvaluateScriptWithNoOpenWebViewShouldFail()
{
	int result;
	result = maWebViewEvaluateScript("foo = 42;");
	assert(WEBVIEW_NOT_OPEN == result);
}

void EvaluateScriptWithErrorsShouldFail()
{
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewEvaluateScript("foo = fortytwo;");
	assert(WEBVIEW_ERROR == result);
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
}

void EvaluateScriptWithNULLValueShouldFail()
{
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewEvaluateScript(NULL);
	assert(WEBVIEW_ERROR == result);
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
}

// ****** Funny Tests ****** //

void SayThanks()
{
	int result;
	result = maWebViewOpen();
	assert(WEBVIEW_OK == result);
	result = maWebViewSetHTML("<html><body>All tests passed! Have a wonderful day (or night)!<script>setTimeout(\"document.location = 'mosync://Done'\", 2000);</script></body></html>");
	assert(WEBVIEW_OK == result);
	MAEvent e;
	while (!maGetEvent(&e));
	TESTIFY_ASSERT(EVENT_TYPE_WEBVIEW_SERVICE_REQUEST == e.type);
	assert(0 == strcmp(e.serviceRequest, "mosync://Done"));
	result = maWebViewClose();
	assert(WEBVIEW_OK == result);
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

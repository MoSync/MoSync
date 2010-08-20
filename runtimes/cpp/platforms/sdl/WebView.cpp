#include "WebView.h"
#include "Syscall.h"

// Start of Windows/Visual Studio specific code

#if defined(_MSC_VER) && (_MSC_VER >= 1020)

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER                // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501         // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT          // Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501   // Change this to the appropriate value to target other versions of Windows.
#endif                        

#ifndef _WIN32_WINDOWS        // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE             // Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600      // Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <SDL/SDL_syswm.h>

#include <webkit/windows/WebKit.h>
#include <webkit/windows/WebKitCOMAPI.h>


// Defined in SyscallImpl.cpp
// TODO: In which file should we put this declaration?
extern void MoSyncPostEvent(MAEvent& e);

// Local functions.
static BSTR privateCharToWideChar(const char* s);
static LPSTR privateWideCharToChar(LPCWSTR s);
static int privateWebViewSetHTML(IWebView* webView, const char* html);

// Forward class declaration.
class PolicyDelegate;

// Are static variables application instance specific?
// If not, they need to be put into an instance specific data structure.

//static HINSTANCE hInst;
static HWND gMainWnd;
static IWebView* gWebView = 0; // We use this variable to check if a WebView is open.
static HWND gViewWindow = 0;
static PolicyDelegate* gPolicyDelegate = 0;
static char* gWebViewRequest = 0; // Hard coded to support one request.

/**
 * Helper to convert a char* to a wide string. 
 * TODO: Specify how returned string should be deallocated.
 */
static BSTR privateCharToWideChar(const char* s)
{
	// First find out how many wide characters are required. 
	int numWideCharsRequired = MultiByteToWideChar(
		CP_ACP, // CodePage
		0, // dwFlags
		s, // String to be converted
		-1, // Assume null terminated
		NULL, 
		0 // Request number of wide characters
	);

	if (0 == numWideCharsRequired) 
	{
		return NULL;
	}

	// Allocate memory for temporary wide string.
	void* tempWideString = malloc((numWideCharsRequired * sizeof(wchar_t)) + 1);
	if (!tempWideString) 
	{
		return NULL;
	}

	// Convert to wide string.
	int numWideChars = MultiByteToWideChar(
		CP_ACP, // CodePage
		0, // dwFlags
		s, // String to be converted
		-1, // Assume null terminated
		(LPWSTR) tempWideString, 
		numWideCharsRequired
	);

	if (0 == numWideChars) 
	{
		return NULL;
	}

	// Sanity check
	if (numWideChars != numWideCharsRequired) 
	{
		return NULL;
	}

	// Allocate string to be passed to the WebFrame.
    BSTR wideString = SysAllocString((const OLECHAR*) tempWideString);

	// Free temporary wide string.
	free(tempWideString);

	// Return allocted wide string.
	return wideString;
}

/** 
 * Helper to convert a wide string to a char*.
 * TODO: Specify how returned string should be deallocated.
 */
static LPSTR privateWideCharToChar(LPCWSTR s)
{
	// First find out how many wide characters are required. 
	int numCharsRequired = WideCharToMultiByte(
		CP_ACP, // CodePage
		0, // dwFlags
		s, // String to be converted
		-1, // Assume null terminated
		NULL, 
		0, // Request number of characters
		NULL, 
		NULL
	);

	if (0 == numCharsRequired) 
	{
		return NULL;
	}

	// Allocate memory for temporary string.
	void* tempString = malloc((numCharsRequired * sizeof(char)) + 1);
	if (!tempString) 
	{
		return NULL;
	}

	// Convert to char string.
	int numChars = WideCharToMultiByte(
		CP_ACP, // CodePage
		0, // dwFlags
		s, // String to be converted
		-1, // Assume null terminated
		(LPSTR) tempString, 
		numCharsRequired,
		NULL,
		NULL
	);

	if (0 == numChars) 
	{
		return NULL;
	}

	// Sanity check
	if (numChars != numCharsRequired) 
	{
		return NULL;
	}

	// Return allocted string.
	return (LPSTR) tempString;
}

/**
 * Helper function that sets the HTML of a WebView.
 */
static int privateWebViewSetHTML(IWebView* webView, const char* html)
{
	// Get the WebFrame of the WebView.
    IWebFrame* frame;
    HRESULT result = webView->mainFrame(&frame);
    if (FAILED(result))
	{
        return WEBVIEW_ERROR;
	}

	// We need to convert the char* string to a wide character string.
	BSTR wideHTML = privateCharToWideChar(html);
	if (!wideHTML)
	{
        return WEBVIEW_ERROR;
	}

	// Should we make use of the base url? (second param)
	// It is assumed that the frame takes ownership of the string wideHTML,
	// therefore we don't free it after the call. TODO: We should check this.
    frame->loadHTMLString(wideHTML, 0);
    frame->Release();

	return WEBVIEW_OK;
}

class PolicyDelegate : public IWebPolicyDelegate
{
public:
    PolicyDelegate() : m_refCount(1) {}

    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

    virtual HRESULT STDMETHODCALLTYPE decidePolicyForNavigationAction( 
        /* [in] */ IWebView *webView,
        /* [in] */ IPropertyBag *actionInformation,
        /* [in] */ IWebURLRequest *request,
        /* [in] */ IWebFrame *frame,
        /* [in] */ IWebPolicyDecisionListener *listener);
    
    virtual HRESULT STDMETHODCALLTYPE decidePolicyForNewWindowAction( 
        /* [in] */ IWebView *webView,
        /* [in] */ IPropertyBag *actionInformation,
        /* [in] */ IWebURLRequest *request,
        /* [in] */ BSTR frameName,
        /* [in] */ IWebPolicyDecisionListener *listener) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE decidePolicyForMIMEType( 
        /* [in] */ IWebView *webView,
        /* [in] */ BSTR type,
        /* [in] */ IWebURLRequest *request,
        /* [in] */ IWebFrame *frame,
        /* [in] */ IWebPolicyDecisionListener *listener) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE unableToImplementPolicyWithError( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebError *error,
        /* [in] */ IWebFrame *frame) { return S_OK; }

protected:
    ULONG m_refCount;
};

HRESULT STDMETHODCALLTYPE PolicyDelegate::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<IWebPolicyDelegate*>(this);
    else if (IsEqualGUID(riid, IID_IWebPolicyDelegate))
        *ppvObject = static_cast<IWebPolicyDelegate*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE PolicyDelegate::AddRef(void)
{
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE PolicyDelegate::Release(void)
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

	return newRef;
}

HRESULT STDMETHODCALLTYPE PolicyDelegate::decidePolicyForNavigationAction( 
        /* [in] */ IWebView *webView,
        /* [in] */ IPropertyBag *actionInformation,
        /* [in] */ IWebURLRequest *request,
        /* [in] */ IWebFrame *frame,
        /* [in] */ IWebPolicyDecisionListener *listener) 
{
	// Get url of request.
	BSTR wideURL;
	request->URL(&wideURL);
	printf("request->URL() -> %S\n", wideURL);

	// Convert to char string.
	char* url = privateWideCharToChar(wideURL);
	if (!url)
	{
		return S_FALSE;
	}

	// Is this a mosync url?
	const char* mosync = "mosync://";
	int result = strncmp(url, mosync, sizeof(mosync));
	if (0 != result)
	{
		// No it is not, do normal processing.
		listener->use();
		return S_OK;
	}

    // Create event.
    MAEvent theEvent;
    theEvent.type = EVENT_TYPE_WEBVIEW_REQUEST;
	// Hard coded request id. 
	// TODO: Change to a circular buffer or something, to support multiple requests.
	theEvent.key = 1; 

	// Store request in a buffer.
	if (gWebViewRequest) { free(gWebViewRequest); gWebViewRequest = 0; }
	gWebViewRequest = (char*) malloc(sizeof(char) * (strlen(url) + 1));
	if (!gWebViewRequest)
	{
		// Could not allocate memory.
		return S_FALSE;
	}
    strcpy(gWebViewRequest, url);

	// We are done with the string.
	free(url);

    // Post in event queue
    MoSyncPostEvent(theEvent);

	// Stop further processing of this url by WebKit.
	listener->ignore();

	return S_OK;
}

int maWebViewOpen(int left, int top, int width, int height)
{
	// Return if there is already an open WebView.
	if (gWebView) 
	{
		return WEBVIEW_OK;
	}

    // Init COM.
    OleInitialize(NULL);

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWMInfo(&info))
	{
		gMainWnd = info.window;
		//printf("Found gMainWnd");
	}
	else
	{
		//printf("NOT Found gMainWnd");
		return WEBVIEW_ERROR;
	}

	// Setting window style WS_CLIPCHILDREN prevents drawing on top of the WebView.
	LONG windowStyle = GetWindowLong(gMainWnd, GWL_STYLE);
	SetWindowLong(gMainWnd, GWL_STYLE, windowStyle | WS_CLIPCHILDREN); // Not needed: WS_CLIPSIBLINGS

    HRESULT hr = WebKitCreateInstance(CLSID_WebView, 0, IID_IWebView, (void**)&gWebView);
    if (FAILED(hr))
	{
        goto exit;
	}

    gPolicyDelegate = new PolicyDelegate();
    gPolicyDelegate->AddRef();
    hr = gWebView->setPolicyDelegate(gPolicyDelegate);
    if (FAILED(hr))
	{
        goto exit;
	}

    hr = gWebView->setHostWindow((OLE_HANDLE) gMainWnd);
    if (FAILED(hr))
	{
        goto exit;
	}

    RECT clientRect;
    GetClientRect(gMainWnd, &clientRect);
    hr = gWebView->initWithFrame(clientRect, 0, 0);
    if (FAILED(hr))
	{
        goto exit;
	}

	// Set default document content.
	privateWebViewSetHTML(gWebView, "<p style=\"background-color: #00FF00\">Hello World</p><div style=\"border: solid blue\">div with blue border</div><ul><li>foo<li>bar<li>baz</ul>");

    IWebViewPrivate* viewExt;
    hr = gWebView->QueryInterface(IID_IWebViewPrivate, (void**)&viewExt);
    if (FAILED(hr))
	{
        goto exit;
	}

    hr = viewExt->viewWindow((OLE_HANDLE*) &gViewWindow);
    viewExt->Release();
    if (FAILED(hr) || !gViewWindow)
	{
        goto exit;
	}
	
    //RECT rcClient;
    //GetClientRect(gMainWnd, &rcClient);
    //MoveWindow(gViewWindow, 0, 0, rcClient.right - 50, rcClient.bottom - 100, TRUE);

	// Position the WebView window.
	MoveWindow(gViewWindow, left, top, width, height, TRUE);

    ShowWindow(gViewWindow, SW_SHOW);
    UpdateWindow(gViewWindow);

	// Post opened event to MoSync.
    MAEvent theEvent;
    theEvent.type = EVENT_TYPE_WEBVIEW_OPENED;
    MoSyncPostEvent(theEvent);

	return WEBVIEW_OK;

exit:
    gWebView->Release();
    shutDownWebKit();
    OleUninitialize();

	return WEBVIEW_ERROR;
}

int maWebViewClose()
{
	// Return if there is not an open WebView.
	if (!gWebView)
	{
		return WEBVIEW_NOT_OPEN;
	}

    gWebView->Release();
    shutDownWebKit();
    OleUninitialize();

	//ShowWindow(gViewWindow, SW_HIDE);
	CloseWindow(gViewWindow);
	DestroyWindow(gViewWindow);

	// Clear global variables.
	gMainWnd = 0;
	gWebView = 0;
	gViewWindow = 0;
	gPolicyDelegate = 0;

	// Free memory for request. 
	// TODO: Move this to its own handler. We have duplicated code now.
	if (gWebViewRequest) { free(gWebViewRequest); gWebViewRequest = 0; }

	// Post closed event to MoSync.
    MAEvent theEvent;
    theEvent.type = EVENT_TYPE_WEBVIEW_CLOSED;
    MoSyncPostEvent(theEvent);

	return WEBVIEW_OK;
}

int maWebViewSetHTML(const char* html)
{
	// Return if there is not an open WebView.
	if (!gWebView)
	{
		return WEBVIEW_NOT_OPEN;
	}

	if (0 == strlen(html))
	{
		return WEBVIEW_ERROR;
	}

	int result = privateWebViewSetHTML(gWebView, html);

	// Post loaded event to MoSync.
	// TODO: Move this to IWebFrameLoadDelegate.didFinishLoadForFrame()
    MAEvent theEvent;
    theEvent.type = EVENT_TYPE_WEBVIEW_PAGE_LOADED;
    MoSyncPostEvent(theEvent);

	return result;
}


int maWebViewLoadURL(const char* url)
{
	// Return if there is not an open WebView.
	if (!gWebView)
	{
		return WEBVIEW_NOT_OPEN;
	}

	if (0 == strlen(url))
	{
		return WEBVIEW_ERROR;
	}

	// Will this work?!
	int result = privateWebViewSetHTML(gWebView, url);

	// Post loaded event to MoSync.
	// TODO: Move this to IWebFrameLoadDelegate.didFinishLoadForFrame()
    MAEvent theEvent;
    theEvent.type = EVENT_TYPE_WEBVIEW_PAGE_LOADED;
    MoSyncPostEvent(theEvent);

	return result;
}

int maWebViewEvaluateScript(const char* script)
{
	// Return if there is not an open WebView.
	if (!gWebView)
	{
		printf("WebView is not open\n");
		return WEBVIEW_NOT_OPEN;
	}

	if (0 == strlen(script))
	{
		return WEBVIEW_ERROR;
	}

	BSTR wideScript = privateCharToWideChar(script);
	if (!wideScript)
	{
		printf("Failed to convert script to wide string: %s\n", script);
        return WEBVIEW_ERROR;
	}

	BSTR returnValue;

	HRESULT result = gWebView->stringByEvaluatingJavaScriptFromString(wideScript, &returnValue);
    if (FAILED(result))
	{
		printf("Failed to evaluate: %S\n", wideScript);
        return WEBVIEW_ERROR;
	}

	printf("Return value: %S\n", returnValue);

	// Should we free string wideScript after the call?
    SysFreeString(wideScript);

	/* Does not work, windowScriptObject returns NULL.
	IWebScriptObject* scriptObject;
	HRESULT result = gWebView->windowScriptObject(&scriptObject);
    if (FAILED(result))
	{
        return WebViewError;
	}
	scriptObject->evaluateWebScript(wideScript, NULL);
	*/

	return WEBVIEW_OK;
}

// TODO: Use requestID to support multiple requests.
int maWebViewGetRequestSize(int requestID)
{
	return strlen(gWebViewRequest) + 1;
}

// TODO: Use requestID to support multiple requests.
int maWebViewGetRequest(int requestID, char* buf, int bufSize)
{
	if ((size_t)bufSize < strlen(gWebViewRequest) + 1)
	{
		return WEBVIEW_ERROR;
	}

	strcpy(buf, gWebViewRequest);

	return strlen(gWebViewRequest) + 1;
}

// End of Windows/VisualStudio specific code

#else

// Start of dummy implementation

int maWebViewOpen(int left, int top, int width, int height)
{
	return WEBVIEW_ERROR;
}

int maWebViewClose()
{
	return WEBVIEW_ERROR;
}

int maWebViewSetHTML(const char* html)
{
	return WEBVIEW_ERROR;
}

int maWebViewLoadURL(const char* url)
{
	return WEBVIEW_ERROR;
}

int maWebViewEvaluateScript(const char* script)
{
	return WEBVIEW_ERROR;
}

int maWebViewGetRequestSize(int requestID)
{
	return WEBVIEW_ERROR;
}

int maWebViewGetRequest(int requestID, char* buf, int bufSize)
{
	return WEBVIEW_ERROR;
}

// End of dummy implementation

#endif

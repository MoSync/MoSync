
// This code is Windows specific.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER                // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501        // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501    // Change this to the appropriate value to target other versions of Windows.
#endif                        

#ifndef _WIN32_WINDOWS        // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE            // Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600    // Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN        // Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <SDL/SDL_syswm.h>

#include <WebKit/WebKit.h>
#include <WebKit/WebKitCOMAPI.h>

#include "WebView.h"

// Forward declarations
class PolicyDelegate;
//class FrameLoadDelegate;

// Are static variables application instance specific?
// If not, they need to be put into an instance specific data structure.

//static HINSTANCE hInst;
static HWND gMainWnd;
static IWebView* gWebView = 0; // We use this variable to check if a WebView is open.
static HWND gViewWindow = 0;
static PolicyDelegate* gPolicyDelegate = 0;
//static FrameLoadDelegate* gFrameLoadDelegate = 0;

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
	printf("decidePolicyForNavigationAction");
	listener->use();
	return S_OK; 
}

#ifdef UNUSED
class FrameLoadDelegate : public IWebFrameLoadDelegate
{
public:
    FrameLoadDelegate() : m_refCount(1) {}

    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);

    // IWebFrameLoadDelegate
    virtual HRESULT STDMETHODCALLTYPE didStartProvisionalLoadForFrame( 
        /* [in] */ IWebView* webView,
        /* [in] */ IWebFrame* /*frame*/) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didReceiveServerRedirectForProvisionalLoadForFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didFailProvisionalLoadWithError( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebError *error,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didCommitLoadForFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didReceiveTitle( 
        /* [in] */ IWebView *webView,
        /* [in] */ BSTR title,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didChangeIcons(
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE didReceiveIcon( 
        /* [in] */ IWebView *webView,
        /* [in] */ OLE_HANDLE hBitmap,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didFinishLoadForFrame( 
        /* [in] */ IWebView* webView,
        /* [in] */ IWebFrame* /*frame*/) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didFailLoadWithError( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebError *error,
        /* [in] */ IWebFrame *forFrame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didChangeLocationWithinPageForFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }

    virtual HRESULT STDMETHODCALLTYPE willPerformClientRedirectToURL( 
        /* [in] */ IWebView *webView,
        /* [in] */ BSTR url,
        /* [in] */ double delaySeconds,
        /* [in] */ DATE fireDate,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE didCancelClientRedirectForFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual HRESULT STDMETHODCALLTYPE willCloseFrame( 
        /* [in] */ IWebView *webView,
        /* [in] */ IWebFrame *frame) { return S_OK; }
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE windowScriptObjectAvailable( 
        /* [in] */ IWebView *webView,
        /* [in] */ JSContextRef context,
        /* [in] */ JSObjectRef windowScriptObject)  { return S_OK; }

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE didClearWindowObject( 
        /* [in] */ IWebView *webView,
        /* [in] */ JSContextRef context,
        /* [in] */ JSObjectRef windowScriptObject,
        /* [in] */ IWebFrame *frame) { return S_OK; }

protected:
    ULONG m_refCount;
};

HRESULT STDMETHODCALLTYPE FrameLoadDelegate::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<IWebFrameLoadDelegate*>(this);
    else if (IsEqualGUID(riid, IID_IWebFrameLoadDelegate))
        *ppvObject = static_cast<IWebFrameLoadDelegate*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE FrameLoadDelegate::AddRef(void)
{
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE FrameLoadDelegate::Release(void)
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

	return newRef;
}
#endif

// Helper to convert a char* to a wide string. 
// TODO: Specify how returned string should be deallocated.
BSTR privateCharToWideChar(const char* s)
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

// Helper function that sets the HTML of a WebView.
static int privateWebViewSetHTML(IWebView* webView, const char* html)
{
	// Get the WebFrame of the WebView.
    IWebFrame* frame;
    HRESULT result = webView->mainFrame(&frame);
    if (FAILED(result))
	{
        return WebViewError;
	}

	// We need to convert the char* string to a wide character string.
	BSTR wideHTML = privateCharToWideChar(html);
	if (!wideHTML)
	{
        return WebViewError;
	}

	// Should we make use of the base url? (second param)
    frame->loadHTMLString(wideHTML, 0);
    frame->Release();

	return WebViewOk;
}

int webViewOpen(int left, int top, int width, int height)
{
	// Return if there is already an open WebView.
	if (gWebView) 
	{
		return WebViewOk;
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
		return WebViewError;
	}

	// Setting window style WS_CLIPCHILDREN prevents drawing on top of the WebView.
	LONG windowStyle = GetWindowLong(gMainWnd, GWL_STYLE);
	SetWindowLong(gMainWnd, GWL_STYLE, windowStyle | WS_CLIPCHILDREN); // WS_CLIPSIBLINGS

    HRESULT hr = WebKitCreateInstance(CLSID_WebView, 0, IID_IWebView, (void**)&gWebView);
    if (FAILED(hr))
        goto exit;
#ifdef UNUSED
    gFrameLoadDelegate = new FrameLoadDelegate();
    gFrameLoadDelegate->AddRef();
    hr = gWebView->setFrameLoadDelegate(gFrameLoadDelegate);
    if (FAILED(hr))
        goto exit;
#endif
    gPolicyDelegate = new PolicyDelegate();
    gPolicyDelegate->AddRef();
    hr = gWebView->setPolicyDelegate(gPolicyDelegate);
    if (FAILED(hr))
        goto exit;

    hr = gWebView->setHostWindow((OLE_HANDLE) gMainWnd);
    if (FAILED(hr))
        goto exit;

    RECT clientRect;
    GetClientRect(gMainWnd, &clientRect);
    hr = gWebView->initWithFrame(clientRect, 0, 0);
    if (FAILED(hr))
        goto exit;

	privateWebViewSetHTML(gWebView, "<p style=\"background-color: #00FF00\">Hello World</p><div style=\"border: solid blue\">div with blue border</div><ul><li>foo<li>bar<li>baz</ul>");

    IWebViewPrivate* viewExt;
    hr = gWebView->QueryInterface(IID_IWebViewPrivate, (void**)&viewExt);
    if (FAILED(hr))
        goto exit;

    hr = viewExt->viewWindow((OLE_HANDLE*) &gViewWindow);
    viewExt->Release();
    if (FAILED(hr) || !gViewWindow)
        goto exit;
	
    //RECT rcClient;
    //GetClientRect(gMainWnd, &rcClient);
    //MoveWindow(gViewWindow, 0, 0, rcClient.right - 50, rcClient.bottom - 100, TRUE);

	// Position the WebView window.
	MoveWindow(gViewWindow, left, top, width, height, TRUE);

    ShowWindow(gViewWindow, SW_SHOW);
    UpdateWindow(gViewWindow);

	return WebViewOk;

exit:
    gWebView->Release();
    shutDownWebKit();
    OleUninitialize();

	return WebViewError;
}

int webViewClose()
{
	// Return if there is NOT an open WebView.
	if (!gWebView)
	{
		return WebViewNotOpen;
	}

    gWebView->Release();
    shutDownWebKit();
    OleUninitialize();

	// Clear global variables.
	gMainWnd = 0;
	gWebView = 0;
	gViewWindow = 0;
	gPolicyDelegate = 0;
	//gFrameLoadDelegate = 0;

	return WebViewOk;
}

int webViewSetHTML(const char* html)
{
	return privateWebViewSetHTML(gWebView, html);
}


// IWebScriptObject
// IWebView
//   virtual HRESULT STDMETHODCALLTYPE windowScriptObject( 
//           /* [retval][out] */ IWebScriptObject **webScriptObject) = 0;
//[win evaluateWebScript:
//    @"addImage(’sample_graphic.jpg’, ‘320’, ‘240’)"];

int webViewEvaluateScript(const char* script)
{
	printf("webViewEvaluateScript 1");

	BSTR wideScript = privateCharToWideChar(script);
	if (!wideScript)
	{
        return WebViewError;
	}

	printf("webViewEvaluateScript 2");

	BSTR returnValue;

	HRESULT result = gWebView->stringByEvaluatingJavaScriptFromString(wideScript, &returnValue);
    if (FAILED(result))
	{
        return WebViewError;
	}

	printf("webViewEvaluateScript 3");

	/*
	IWebScriptObject* scriptObject;
	HRESULT result = gWebView->windowScriptObject(&scriptObject);
    if (FAILED(result))
	{
        return WebViewError;
	}
	scriptObject->evaluateWebScript(wideScript, NULL);
	*/

	return WebViewOk;
}

#ifdef DONT_USE
		case maIOCtl_maWebViewOpen:
		{
            char *  data;
            MAEvent myevent;
            
            // Get pointer to memory
            data = (char*)SYSCALL_THIS->GetValidatedMemRange( a, 16 );

            // Copy to event
            myevent.type = EVENT_TYPE_WEBKIT;
            memcpy( myevent.webKitEventData.m_data, data, 16 );

            // Put in event queue
            gEventFifo.put( myevent );

            // Return success code
            return 1;   
        }
		
		default:
			return IOCTL_UNAVAILABLE;
		}
	}	//maIOCtl
#endif

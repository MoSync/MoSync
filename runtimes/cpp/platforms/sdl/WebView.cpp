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

class WinLauncherWebHost;

//static HINSTANCE hInst;
static HWND hMainWnd;
static IWebView* gWebView = 0;
static HWND gViewWindow = 0;
static WinLauncherWebHost* gWebHost = 0;

class WinLauncherWebHost : public IWebFrameLoadDelegate
{
public:
    WinLauncherWebHost() : m_refCount(1) {}

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

    // WinLauncherWebHost

protected:
    ULONG                   m_refCount;
};

HRESULT STDMETHODCALLTYPE WinLauncherWebHost::QueryInterface(REFIID riid, void** ppvObject)
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

ULONG STDMETHODCALLTYPE WinLauncherWebHost::AddRef(void)
{
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE WinLauncherWebHost::Release(void)
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

	return newRef;
}

int webViewOpen()
{
    // Init COM.
    OleInitialize(NULL);

    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWMInfo(&info))
	{
		hMainWnd = info.window;
		printf("Found hMainWnd");
	}
	else
	{
		printf("NOT Found hMainWnd");
		return 1;
	}

	// Does not seem to fix the overdraw problem.
	LONG windowStyle = GetWindowLong(hMainWnd, GWL_STYLE);
	SetWindowLong(hMainWnd, GWL_STYLE, windowStyle | WS_CLIPCHILDREN); // WS_CLIPSIBLINGS

    //HWND hMainWnd = FindWindow(NULL, "MoSync");

    HRESULT hr = WebKitCreateInstance(CLSID_WebView, 0, IID_IWebView, (void**)&gWebView);
    if (FAILED(hr))
        goto exit;

    gWebHost = new WinLauncherWebHost();
    gWebHost->AddRef();
    hr = gWebView->setFrameLoadDelegate(gWebHost);
    if (FAILED(hr))
        goto exit;

    hr = gWebView->setHostWindow((OLE_HANDLE) hMainWnd);
    if (FAILED(hr))
        goto exit;

    RECT clientRect;
    GetClientRect(hMainWnd, &clientRect);
    hr = gWebView->initWithFrame(clientRect, 0, 0);
    if (FAILED(hr))
        goto exit;

    IWebFrame* frame;
    hr = gWebView->mainFrame(&frame);
    if (FAILED(hr))
        goto exit;

    // static BSTR defaultHTML = SysAllocString(TEXT("<p style=\"background-color: #00FF00\">Testing</p><img src=\"http://webkit.org/images/icon-gold.png\" alt=\"Face\"><div style=\"border: solid blue\" contenteditable=\"true\">div with blue border</div><ul><li>Miki was here! foo<li>bar<li>baz</ul>"));
    static BSTR defaultHTML = SysAllocString(L"<p style=\"background-color: #00FF00\">Testing</p><div style=\"border: solid blue\" contenteditable=\"true\">div with blue border</div><ul><li>foo<li>bar<li>baz</ul>");
    frame->loadHTMLString(defaultHTML, 0);
    frame->Release();

    IWebViewPrivate* viewExt;
    hr = gWebView->QueryInterface(IID_IWebViewPrivate, (void**)&viewExt);
    if (FAILED(hr))
        goto exit;

    hr = viewExt->viewWindow((OLE_HANDLE*) &gViewWindow);
    viewExt->Release();
    if (FAILED(hr) || !gViewWindow)
        goto exit;
	
    RECT rcClient;
    GetClientRect(hMainWnd, &rcClient);
    MoveWindow(gViewWindow, 0, 0, rcClient.right - 50, rcClient.bottom - 100, TRUE);

    ShowWindow(gViewWindow, SW_SHOW);
    UpdateWindow(gViewWindow);

	return 1;

exit:
    gWebView->Release();
    shutDownWebKit();
    OleUninitialize();

	return 1;
}

int webViewClose()
{
    gWebView->Release();
    shutDownWebKit();
    OleUninitialize();

	return 1;
}

int webViewSetHTML(int a)
{
	return 1;
}

int webViewEvaluateScript(int a)
{
	return 1;
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

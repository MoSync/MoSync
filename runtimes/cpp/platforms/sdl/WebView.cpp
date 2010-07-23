#include "WebView.h"

int webViewOpen()
{
	return 1;
}

int webViewClose()
{
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

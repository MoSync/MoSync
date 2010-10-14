#ifndef WIDGETMESSAGEUTIL_H
#define WIDGETMESSAGEUTIL_H

#include <IX_WIDGET.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate data.
 */
char* WidgetMessageAllocateString(int size);

/**
 * Free data.
 */
void WidgetMessageFreeString(char* data);

/**
 * Get the message string from a message event.
 * Deallocate the string with widgetMessageFreeData().
 */
char* WidgetMessageGet(int messageId);

/**
 * Return true (non-zero) if the name part of the message
 * data matches the given name. The message has the format:
 * MessageName/MessageData
 */
int WidgetMessageNameIs(char* message, char* messageName);

/**
 * Return the name part of a message that has the form:
 * MessageName/MessageData
 * The returned string must be deallocated with maWidgetFreeMessageData.
 * On error, NULL is returned.
 */
char* WidgetMessageGetName(char* message);

/**
 * Return the data part of a message that has the form:
 * MessageName/MessageData
 * The returned string must be deallocated with maWidgetFreeMessageData.
 * On error, NULL is returned.
 */
char* WidgetMessageGetData(const char* message);

// TODO: If needed, implement functions that call JS to 
// go back and forward. We could implement several similar
// functions that call JS do accomplish common tasks.
//int WebViewGoBack(MAHandle widgetHandle);
//int WebViewGoForward(MAHandle widgetHandle);

/**
 * Convert url encoded string to ascii string (or wide char string).
 */
//TODO: char* widgetMessageDecode(char* message);
// --> Or maybe even better, let the syscall maWidgetGetMessage() do this.

#ifdef __cplusplus
}
#endif

#endif

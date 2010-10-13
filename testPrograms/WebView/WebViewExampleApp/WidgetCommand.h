#ifndef WIDGETCOMMAND_H
#define WIDGETCOMMAND_H

#include <IX_WIDGET.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocate data.
 */
char* WidgetCommandAllocateString(int size);

/**
 * Free data.
 */
void WidgetCommandFreeString(char* data);

/**
 * Get the command string from a command event.
 * Deallocate the string with widgetCommandFreeData().
 */
char* WidgetCommandGet(int commandId);

/**
 * Return true (non-zero) if the name part of the command
 * data matches the given name. The command has the format:
 * CommandName/CommandData
 */
int WidgetCommandNameIs(char* command, char* commandName);

/**
 * Return the name part of a command that has the form:
 * CommandName/CommandData
 * The returned string must be deallocated with maWidgetFreeCommandData.
 * On error, NULL is returned.
 */
char* WidgetCommandGetName(char* command);

/**
 * Return the data part of a command that has the form:
 * CommandName/CommandData
 * The returned string must be deallocated with maWidgetFreeCommandData.
 * On error, NULL is returned.
 */
char* WidgetCommandGetData(const char* command);

// TODO: If needed, implement functions that call JS to 
// go back and forward. We could implement several similar
// functions that call JS do accomplish common tasks.
//int WebViewGoBack(MAHandle widgetHandle);
//int WebViewGoForward(MAHandle widgetHandle);

/**
 * Convert url encoded string to ascii string (or wide char string).
 */
//TODO: char* widgetCommandDecode(char* command);
// --> Or maybe even better, let the syscall maWidgetGetCommand() do this.

#ifdef __cplusplus
}
#endif

#endif

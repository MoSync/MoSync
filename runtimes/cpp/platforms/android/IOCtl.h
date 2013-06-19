/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <Syscall.h>
#include "JNIStrConv.h"

namespace Base
{

	// Framebuffer API

	int _maFrameBufferGetInfo(MAFrameBufferInfo *info);

	int _maFrameBufferInit(void *data, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maFrameBufferClose(JNIEnv* jNIEnv, jobject jThis);

	// Bluetooth API

	int _maBtStartDeviceDiscovery(int names, JNIEnv* jNIEnv, jobject jThis);

	int _maBtGetNewDevice(
		int memStart,
		int nameBufPointer,
		int nameBufSize,
		int actualNameLengthPointer,
		int addressPointer,
		JNIEnv* jNIEnv,
		jobject jThis);

	int _maBtStartServiceDiscovery(
		MABtAddr* addressPointer,
		MAUUID* uuidPointer,
		JNIEnv* jNIEnv,
		jobject jThis);

	int _maBtGetNextServiceSize(
		int memStart,
		int nameBufSizePointer,
		int nUuidsPointer,
		JNIEnv* jNIEnv,
		jobject jThis);

	int _maBtGetNewService(
		int memStart,
		int portPointer,
		int nameBufPointer,
		int nameBufSize,
		int uuidsPointer,
		JNIEnv* jNIEnv,
		jobject jThis);

	int _maBtCancelDiscovery(JNIEnv* jNIEnv, jobject jThis);

	// Server API

	int _maAccept(int serverHandle, JNIEnv* jNIEnv, jobject jThis);

	// Location API

	int _maLocationStart(JNIEnv* jNIEnv, jobject jThis);

	int _maLocationStop(JNIEnv* jNIEnv, jobject jThis);

	// SMS API

	int _maSendTextSMS(
		const char* phoneNo,
		const char* message,
		JNIEnv* jNIEnv,
		jobject jThis);

	// Other IOCTLs

	int _maGetSystemProperty(
		const char* key,
		int buf,
		int memStart,
		int size,
		JNIEnv* jNIEnv,
		jobject jThis);

	int _maPlatformRequest(const char* url, JNIEnv* jNIEnv, jobject jThis);

	int _maWriteLog(const char* str, int b, JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Internal function corresponding to the maShowVirtualKeyboard IOCtl.
	 * Shows the android soft keyboard.
	 *
	 * @param jNIEnv	JNI environment used
	 * @param jThis		Pointer to the java class
	 *
	 * @return			Value returned by the maTextBox
	 *					java method
	 */
	int _maShowVirtualKeyboard(JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Internal function corresponding to the maTextBox IOCtl.
	 * Displays a full screen editable text field with
	 * OK and Cancel buttons.
	 *
	 * @param title			Title of the text box
	 * @param inText		Initial content of the text box
	 * @param outText		Buffer that will contain the text
	 *						entered by the user
	 * @param maxSize		Maximum size of outText
	 * @param constraints	Not implemented yet
	 * @param memStart		Pointer to the begining of the
	 *						MoSync memory
	 * @param jNIEnv		JNI environment used
	 * @param jThis			Pointer to the java class
	 *
	 * @return				Value returned by the maTextBox
	 *						java method
	 */
	int _maTextBox(
		const wchar* title,
		const wchar* inText,
		int outText,
		int maxSize,
		int constraints,
		int memStart,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Add a notification item.
	 *
	 * Note that there can only be one notification of type
	 * NOTIFICATION_TYPE_APPLICATION_LAUNCHER. Additional notification
	 * types may be added in the future. This syscall is available
	 * on Android only.
	 *
	 * @param type The \link #NOTIFICATION_TYPE_APPLICATION_LAUNCHER
	 * \endlink constant.
	 * @param id The id of the notification. The id must be unique within
	 * the application.
	 * @param title Title of the notification.
	 * @param text String to be displayed as part of the notification.
	 * @return \< 0 on error or if the syscall is not available on the
	 * current platform.
	 */
	int _maNotificationAdd(
		int type,
		int id,
		const char* title,
		const char* text,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Remove a notification item.
	 * @param id The id of the notification.
	 * @return \< 0 on error.
	 */
	int _maNotificationRemove(int id, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Sends the application to the background, unless it's already there.
	* Generates a \link #EVENT_TYPE_FOCUS_LOST FOCUS_LOST \endlink event.
	* \note Only available on multi-tasking operating systems.
	*/
	int _maSendToBackground(JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Set the screen orientation.
	 * @param orientation One of the \link #SCREEN_ORIENTATION
	 * \endlink constants.
	 * @return \< 0 on error.
	 */
	int _maScreenSetOrientation(int orientation, JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Enable/disable fullscreen mode.
	 * @param fullscreen 1 for fullscreen on, 0 for fullscreen off.
	 * @return \< 0 on error.
	 */
	int _maScreenSetFullscreen(int fullscreen, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Set supported screen orientations.
	* @param orientations A bitmask consisting of flags describing the
	* supported screen orientations.
	* @return \< 0 on error.
	*/
	int _maScreenSetSupportedOrientations(int orientations, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Get supported screen orientations.
	* @return A bitmask consisting of flags describing the supported screen orientations.
	*/
	int _maScreenGetSupportedOrientations(JNIEnv* jNIEnv, jobject jThis);

	/**
	* Get current screen orientation.
	* @return One of the \link #MA_SCREEN_ORIENTATION_PORTRAIT MA_SCREEN_ORIENTATION \endlink constants.
	*/
	int _maScreenGetCurrentOrientation(JNIEnv* jNIEnv, jobject jThis);

	/**
	* Turn on/off sending of HomeScreen events. Off by default.
	* @param eventsOn 1 = events on, 0 = events off
	* @return \< 0 on error.
	*/
	int _maHomeScreenEventsOnOff(int eventsOn, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Add shortcut icon to the device home screen.
	* Available on Android only.
	* @return \< 0 on error.
	*/
	int _maHomeScreenShortcutAdd(const char* name,
								JNIEnv* jNIEnv, jobject jThis);

	/**
	* Remove shortcut icon from the device home screen.
	* Available on Android only.
	* @return \< 0 on error.
	*/
	int _maHomeScreenShortcutRemove(const char* name,
									JNIEnv* jNIEnv, jobject jThis);

	/**
	* Turn on/off sending of screen on/off events. Off by default.
	* @param eventsOn 1 = events on, 0 = events off
	* @return \< 0 on error.
	*/
	int _maScreenStateEventsOnOff(int eventsOn, JNIEnv* jNIEnv, jobject jThis);

	int _maWakeLock(int flag, JNIEnv* jNIEnv, jobject jThis);

	int _maTextBox(const wchar* title, const wchar* inText, int outText,
					int maxSize, int constraints, int memStart,
					JNIEnv* jNIEnv, jobject jThis);

	int _maMessageBox(const char* title, const char* text,
						JNIEnv* jNIEnv, jobject jThis);

	int _maAlert(const char* title, const char* message, const char* button1,
					const char* button2, const char* button3, JNIEnv* jNIEnv, jobject jThis);

	int _maToast(const char* message, int duration, JNIEnv* jNIEnv, jobject jThis);

	int _maImagePickerOpen(JNIEnv* jNIEnv, jobject jThis);

	int _maImagePickerOpenWithEventReturnType(int eventReturnType, JNIEnv* jNIEnv, jobject jThis);

	int _maOptionsBox(const wchar* title, const wchar* destructiveText, const wchar* cancelText, int bufPointer, int bufSize,
						JNIEnv* jNIEnv, jobject jThis);

	// Widget API
	int _maWidgetCreate(const char *widgetType,
						JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetDestroy(int widget, JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetAddChild(int parent, int child,
							JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetInsertChild(int parent, int child, int index,
							JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetRemoveChild(int child,
							JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetModalDialogShow(int dialog, JNIEnv* jNIEnv, jobject jThis);

	int _maWidgetModalDialogHide(int dialog, JNIEnv* jNIEnv, jobject jThis);

	int _maWidgetSetProperty(int widget, const char *property,
							const char* value,
							JNIEnv* jNIEnv, jobject jThis);

	int _maWidgetGetProperty(int memStart, int widget, const char *property,
								int memBuffer, int memBufferSize,
								JNIEnv* jNIEnv, jobject jThis);
	int _maWidgetStackScreenPush(int stackScreenWidget, int newScreen, JNIEnv* jNIEnv, jobject jThis);

	int _maWidgetStackScreenPop(int stackScreenWidget, JNIEnv* jNIEnv, jobject jThis);

	int _maWidgetScreenAddOptionsMenuItem(int widget, const char* title,
											const char* iconPath, int iconPredefined,
											JNIEnv* jNIEnv, jobject jThis);

	int _maWidgetScreenShow(int screenWidget, JNIEnv* jNIEnv, jobject jThis);

	int _maWidgetScreenShowWithTransition(int screenWidget, int screenTransitionType, int screenTransitionDuration,
											JNIEnv* jNIEnv, jobject jThis);

	// Action Bar
	int _maActionBarSetEnabled(int state, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarAddMenuItem(int screenHandle, const char* title, int iconPredefinedId, int iconHandle,
								int displayFlag, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarRemoveMenuItem(int screenHandle, int itemHandle, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarSetVisibility(int visibility, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarGetHeight(JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarIsShowing(JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarSetTitle(const char* title, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarSetIcon(int iconHandle, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarSetDisplayHomeAsUpEnabled(int enableUp, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarShowTitleEnabled(int enable, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarShowLogoEnabled(int enable, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarSetHomeButtonEnabled(int state, JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarRefresh(JNIEnv* jNIEnv, jobject jThis);
	int _maActionBarSetBackgroundImage(int imageHandle, JNIEnv* jNIEnv, jobject jThis);

	// Open GL API
	/**
	 * Internal function that initializes an Open GL
	 * fullscreen surface.
	 */
	int _maOpenGLInitFullscreen(int glApi, JNIEnv* jNIEnv, jobject jThis);
	/**
	 * Internal function that destroys an Open GL fullscreen
	 * surface.
	 */
	int _maOpenGLCloseFullscreen(JNIEnv* jNIEnv, jobject jThis);
	/**
	 * Internal function that calls the java side to load an opengl
	 * texture, to the currently bound texture handle.
	 *
	 * @param image Image to load.
	 */
	int _maOpenGLTexImage2D(MAHandle image, JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Internal function that calls the java side to load part of an image
	 * into an opengl texture, to the currently bound texture handle.
	 *
	 * @param image Image to load.
	 */
	int _maOpenGLTexSubImage2D(MAHandle image, JNIEnv* jNIEnv, jobject jThis);

	// File API
	/**
	* Opens a file handle.
	* If the file exists, the file will be accessible
	* according to the mode specified.
	* If the file does not exist, a handle will still be returned and
	* maFileExists() will return false.
	* The file may then be created by calling maFileCreate().
	*
	* Directories may be opened with this function.
	* They cannot be read or written, but they can be created and deleted,
	* and their existence can be queried.
	* Directory paths must end with a slash('/').
	*
	* \note To avoid memory leaks, all opened handles should be closed
	* using maFileClose().
	*
	* \note To find valid file paths, use maFileListStart().
	*
	* \param path The absolute path to the file.
	* \param mode The access mode.
	* Either #MA_ACCESS_READ or #MA_ACCESS_READ_WRITE.
	* Directories also use these access modes. #MA_ACCESS_READ lets you
	* see if the directory exists.
	* #MA_ACCESS_READ_WRITE also lets you create or delete it.
	* \returns The new file handle, or \< 0 on error.
	*
	* \note If any maFile function other than maFileOpen() fails,
	* the state of the file handle becomes boundedly undefined.
	* This means that any operation on it, other than maFileClose(), may fail.
	*/
	MAHandle _maFileOpen(const char* path, int mode,
						JNIEnv* jNIEnv, jobject jThis);

	/**
	* Returns 1 if the file exists, 0 if it doesn't, or \< 0 on error.
	*/
	int _maFileExists(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Closes a file handle. Returns 0. Panics on error.
	*/
	int _maFileClose(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	//see JSR 75 for docs
	/**
	* Creates a file or directory, given an open handle.
	* The file must not exist prior to calling this function.
	* After creation, the file will be accessible according to the
	* mode specified when its handle was opened.
	* Returns 0 on success, or \< 0 on error.
	*/
	int _maFileCreate(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Deletes a file or directory.
	* The file must exist prior to calling this function.
	* If it's a directory, it must be empty.
	* Returns 0 on success, or \< 0 on error.
	*/
	int _maFileDelete(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Returns the size of the file, or \< 0 on error.
	* \note MoSync cannot handle files larger than 2 GiB (2^31 - 1 bytes).
	* \param file A file handle.
	* The file must exist and must not be a directory.
	*/
	int _maFileSize(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Returns the number of free bytes on the file system on
	* which \a file resides.
	* Note that due to file system overhead, it is not possible to use all of
	* this space for file data.
	* Returns \< 0 on error.
	*/
	int _maFileAvailableSpace(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Returns the total size, in bytes, of the file system
	* on which \a file resides.
	* Returns \< 0 on error.
	*/
	int _maFileTotalSpace(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Returns the date/time when the file was last modified, or \< 0 on error.
	* The time format is Unix UTC.
	* \see maTime()
	*/
	int _maFileDate(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Renames a file.
	*
	* If \a newName does not contain a slash,
	* the file stays in its original directory.
	*
	* If \a newName does contain a slash, it must be a complete pathname,
	* and the file is moved to that path. The new path must be on the
	* same file system as the original path. If it is not, this function
	* fails with #MA_FERR_RENAME_FILESYSTEM.
	* Moving a file to another directory is not supported on Java ME.
	* If it is attempted, this function fails with #MA_FERR_RENAME_DIRECTORY.
	*
	* \returns 0 on success, or \< 0 on error.
	*/
	int _maFileRename(MAHandle file, const char* newName,
						JNIEnv* jNIEnv, jobject jThis);

	/**
	* Truncates the file.
	*
	* If offset is less than the file's size,
	* the file's new size is equal to the offset.
	* All data after the offset is discarded.
	*
	* If the offset is equal to the file's size, this function does nothing.
	*
	* If the offset is greater than the file's current size,
	* this function MAY increase the size of the file.
	* You can determine if it did so by calling maFileSeek() with #MA_SEEK_END.
	*
	* If the file's position is greater than the offset, it is reset
	* to be equal to the offset.
	*
	* The file must exist prior to calling this function.
	* Returns 0 on success, or \< 0 on error.
	*/
	int _maFileTruncate(MAHandle file, int offset,
						JNIEnv* jNIEnv, jobject jThis);

	/**
	* Write from memory to file.
	* Returns 0 on success, or \< 0 on error.
	*/
	int _maFileWrite(MAHandle file, int src, int len, int memStart,
					JNIEnv* jNIEnv, jobject jThis);
	/**
	* Write from data object to file.
	* Returns 0 on success, or \< 0 on error.
	*/
	int _maFileWriteFromData(MAHandle file, MAHandle data, int offset, int len,
							JNIEnv* jNIEnv, jobject jThis);
	/**
	* Read from file to memory.
	* Returns 0 on success, or \< 0 on error.
	*/
	// dst -> out
	int _maFileRead(MAHandle file, int dst, int len, int memStart,
					JNIEnv* jNIEnv, jobject jThis);
	/**
	* Read from file to data object.
	* Returns 0 on success, or \< 0 on error.
	*/
	int _maFileReadToData(MAHandle file, MAHandle data, int offset, int len,
							JNIEnv* jNIEnv, jobject jThis);

	/**
	* Returns the file's current position, or \< 0 on error.
	* \param file A file handle. The file must exist and must not be a dir.
	*/
	int _maFileTell(MAHandle file, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Sets the file's position.
	* \param file A file handle. The file must exist and must not be a dir.
	* \param offset The offset of the new position,
	* relative to the position specified by \a whence.
	* \param whence One of the \link #MA_SEEK_SET MA_SEEK \endlink constants.
	* \returns The new position, or \< 0 on error.
	*/
	int _maFileSeek(MAHandle file, int offset, int whence,
					JNIEnv* jNIEnv, jobject jThis);

	/**
	* Creates a listing of names of files and directories, or file systems.
	* Call maFileListNext() repeatedly to retrieve the names.
	* Call maFileListClose() to free the resources used.
	*
	* It is often useful to begin by listing the file systems.
	*
	* \param path The full path to a directory, or the empty string,
	* which specifies that the root file systems should be listed.
	* \param filter A string to match names with. May include an asterisk ('*')
	* wildcard to represent 0 or more characters. Ignored if \a path is empty.
	*
	* \returns A File Listing handle, or \< 0 on error.
	*/
	MAHandle _maFileListStart(const char* path, const char* filter,
								JNIEnv* jNIEnv, jobject jThis);

	/**
	* Writes the name of the next file in the list to the specified buffer.
	*
	* The buffer may be too small to contain the name and the terminating zero;
	* in that case, the internal list pointer remains unchanged,
	* the buffer will not be filled,
	* and you should allocate a bigger buffer and call this function again.
	* You may call this function with NULL and 0 to simply retrieve the
	* length of the name.
	*
	* If the name ends with a slash('/'), it is a directory.
	*
	* \returns The length of the name, excluding the terminating zero,
	* or 0 if there are no more files, or \< 0 on error.
	*/
	int _maFileListNext(MAHandle list, int nameBuf, int bufSize, int memStart,
						JNIEnv* jNIEnv, jobject jThis);

	/**
	* Closes a file listing, freeing its resources.
	*
	* \returns 0.
	*/
	int _maFileListClose(MAHandle list, JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Saves an image in the photo gallery of the device.
	 *
	 * returns #MA_MEDIA_RES_OK if the input was correct and the operation started
	 * or #MA_MEDIA_RES_IMAGE_EXPORT_FAILED otherwise.
	 */
	int _maSaveImageToDeviceGallery(
			MAHandle imageHandle,
			const char* imageName,
			JNIEnv* jNIEnv,
			jobject jThis);

	/**
	* Returns a handle to one of the default fonts of the device, in the style and size you specify.
	* \param 'type' The type of the font, can be FONT_TYPE_[SANS_SERIF,SERIF,MONOSPACE].
	* \param 'style' The style of the font, can be FONT_STYLE_[NORMAL,BOLD,ITALIC].
	* \param 'size' The size of the font.
	* \return The handle to the font, RES_FONT_NO_TYPE_STYLE_COMBINATION, or RES_FONT_INVALID_SIZE.
	*/
	MAHandle _maFontLoadDefault(
			int type,
			int style,
			int size,
			JNIEnv* jNIEnv,
			jobject jThis);

	/**
	* Sets the font to be used with maDrawText and maDrawTextW, and returns the handle
	* to the previous font
	* \param 'font' an MAHandle for a font object
	* \return The handle to the previous font, or RES_FONT_INVALID_HANDLE
	*/
	MAHandle _maFontSetCurrent(MAHandle font,JNIEnv* jNIEnv, jobject jThis);

	/**
	* Returns the number of fonts that are available in the system
	* \return Number of fonts
	* \see maFontGetName
	*/
	int _maFontGetCount(JNIEnv* jNIEnv, jobject jThis);

	/**
	* Copies the font postscript name of the given index to the buffer.
	* You must have first called maFontGetCount() at least once before calling this function.
	* \param 'index' A 0-based index to the font
	* \param 'buffer' An empty char buffer that will receive the font name
	* \param 'bufferLen' The size of the buffer
	* \return The number of bytes copied (with terminating NULL) or RES_FONT_INDEX_OUT_OF_BOUNDS
	* 		or RES_FONT_INSUFFICIENT_BUFFER or RES_FONT_LIST_NOT_INITIALIZED
	* \see maFontGetCount, maFontLoadWithName
	*/
	int _maFontGetName(int memStart,
			int index,
			int memBuffer,
			int memBufferSize,
			JNIEnv* jNIEnv,
			jobject jThis);

	/**
	* Returns a handle to a font with the specific postscript name and size
	* \param 'postScriptName' The postscript name of the font
	* \param 'size' The size of the font
	* \return Handle to the font, RES_FONT_NAME_NONEXISTENT, or RES_FONT_INVALID_SIZE.
	*/
	MAHandle _maFontLoadWithName(
			const char* postScriptName,
			int size,
			JNIEnv* jNIEnv,
			jobject jThis);

	/**
	* Deletes a loaded font
	* \param 'font' A font handle
	* \return RES_FONT_OK or RES_FONT_INVALID_HANDLE
	*/
	int _maFontDelete(MAHandle font, JNIEnv* jNIEnv, jobject jThis);

	/**
	* \brief Enables a sensor and starts sending events if the sensor is available.
	* If the sensor already is enabled this call will have no effect.
	* \param sensor       The sensor which should be enabled
	* \param interval     Time interval in which a sensor update shall be triggered
	*            >0 interval time in milliseconds
	*            <=0 Any of SensorUpdate enums
	* \return     0  on success
	*            -1 if this sensor wasn�t available
	*            -2 if the interval wasn�t set, could be due to platform limitations
	*            -3 if the sensor already was enabled
	*/
	int _maSensorStart(int sensor, int interval, JNIEnv* jNIEnv, jobject jThis);

	/**
	* \brief Disables a sensor so that it doesn�t send any further events
	* \param sensor    The sensor which should be disabled
	* \return        0 on success
	*               -1 if this sensor wasn�t enabled
	*            	-2 if there was a problem disabling the sensor
	*/
	int _maSensorStop(int sensor, JNIEnv* jNIEnv, jobject jThis);

	/** Opens a PimList. Use maPimListNext() to open the list's items.
	* Returns a PimList handle, or \< 0 on error.
	* \param listType One of the \link #MA_PIM_CONTACTS MA_PIM \endlink constants.
	*/
	MAHandle _maPimListOpen(int listType, JNIEnv* jNIEnv, jobject jThis);

	/** Returns a handle to the next PimItem in the \a list, or 0 if there are no more items,
	* or \< 0 on error.
	*
	* You must use maPimItemClose() on every item to prevent memory leaks.
	*/
	MAHandle _maPimListNext(MAHandle list, JNIEnv* jNIEnv, jobject jThis);

	/** Returns a handle to the next PimItem in the \a list, or 0 if there are no more items,
	* or \< 0 on error.
	* Using this you can only get name and phone numbers when calling maPimItemGetValue.
	* You must use maPimItemClose() on every item to prevent memory leaks.
	*/
	MAHandle _maPimListNextSummary(MAHandle list, JNIEnv* jNIEnv, jobject jThis);

	/** Closes a PimList.
	* \note This does not close the list's items, but it does invalidate them,
	* so that maPimItemClose() is the only function you can safely use on them.
	* \returns 0.
	*/
	int _maPimListClose(MAHandle list, JNIEnv* jNIEnv, jobject jThis);

	/** Returns the number of fields in the \a item.
	*/
	int _maPimItemCount(MAHandle item, JNIEnv* jNIEnv, jobject jThis);

	/** Returns the field id of the item's n:th field.
	* Panics on invalid indices.
	* A field id is one of the \link #MA_PIM_FIELD_CONTACT_ADDR MA_PIM_FIELD \endlink constants.
	* \param item Opened by maPimListNext().
	* \param n \>= 0 and \< maPimItemCount().
	*/
	int _maPimItemGetField(MAHandle item, int n, JNIEnv* jNIEnv, jobject jThis);

	/** Returns the number of values in the field for a given item.
	* Returns 0 if the field (does not exist / has no values) for this item.
	* \param item Opened by maPimListNext().
	* \param field One of the \link #MA_PIM_FIELD_CONTACT_ADDR MA_PIM_FIELD \endlink constants.
	*/
	int _maPimItemFieldCount(MAHandle item, int field, JNIEnv* jNIEnv, jobject jThis);

	/** Returns the bitmask of attributes for the specified value in a field and item.
	* Panics on field+index combinations that don't exist in this item.
	* \param item Opened by maPimListNext().
	* \param field One of the \link #MA_PIM_FIELD_CONTACT_ADDR MA_PIM_FIELD \endlink constants.
	* \param index \>= 0 and \< maPimItemFieldCount().
	*/
	int _maPimItemGetAttributes(MAHandle item, int field, int index, JNIEnv* jNIEnv, jobject jThis);

	/** Sets a custom label for a value in a field and item.
	* This also removes all of that value's attributes.
	* The label has #MA_PIM_TYPE_STRING and is stored in \a args.buf.
	* \a args.bufSize must be valid.
	* \param args Common arguments.
	* \param index An index into the field's value array.
	* This is a number \>= 0 and \< the return value of maPimItemFieldCount().
	* \returns One of the \link #MA_PIM_ERR_NONE MA_PIM_ERR \endlink constants.
	* \note This function is not available on JavaME or Blackberry.
	*/
	int _maPimItemSetLabel(int item, int field, int bufPointer, int bufSize, int index, JNIEnv* jNIEnv, jobject jThis);

	/** Retrieves the custom label for a value in a field and item,
	* if the value has a custom label.
	* The label has #MA_PIM_TYPE_STRING and is stored in \a args.buf.
	* \a args.bufSize must be valid.
	* \param args Common arguments.
	* \param index An index into the field's value array.
	* This is a number \>= 0 and \< the return value of maPimItemFieldCount().
	* \returns If the value does not have a custom label, #MA_PIM_ERR_NO_LABEL.
	* If it does, the number of bytes occupied by the value.
	* If this number is greater than bufSize, the value is not written,
	* and you'll have to allocate a bigger buffer and try again.
	* \note This function is not available on JavaME or Blackberry.
	*/
	int _maPimItemGetLabel(int item, int field, int bufPointer, int bufSize, int index, JNIEnv* jNIEnv, jobject jThis);

	/** Returns one of the \link #MA_PIM_TYPE_BINARY MA_PIM_TYPE \endlink constants,
	* or \< 0 if the field does not exist in the list.
	* \param list Opened by maPimListOpen().
	* \param field One of the \link #MA_PIM_FIELD_CONTACT_ADDR MA_PIM_FIELD \endlink constants.
	*/
	int _maPimFieldType(MAHandle list, int field, JNIEnv* jNIEnv, jobject jThis);

	/**
	* Copies the specified value from a field and item to \a args.buf.
	* \param args Common arguments.
	* \param index An index into the field's value array.
	* This is a number \>= 0 and \< the return value of maPimItemFieldCount().
	* \returns The number of bytes occupied by the value.
	* If this number is greater than bufSize, the value is not written,
	* and you'll have to allocate a bigger buffer and try again.
	*/
	int _maPimItemGetValue(int item, int field, int bufPointer, int bufSize, int index, JNIEnv* jNIEnv, jobject jThis);

	/** Changes a value and its attributes in a field and item.
	* This function cannot be used to add a new value.
	* \param args Common arguments.
	* \param index An index into the field's value array.
	* This is a number \>= 0 and \< the return value of maPimItemFieldCount().
	* \param attributes The value's new attributes.
	* \returns 0.
	* \note The new value is not actually written to disk until
	* the item is closed with maPimItemClose(). If the program exits before then,
	* the changes are lost.
	*/
	int _maPimItemSetValue(int item, int field, int bufPointer, int bufSize, int index, int attributes, JNIEnv* jNIEnv, jobject jThis);

	/** Adds a new value to a field and item.
	* \param args Common arguments.
	* \param attributes The new value's attributes.
	* \returns 0.
	* \note The new value is not actually written to disk until
	* the item is closed with maPimItemClose(). If the program exits before then,
	* the changes are lost.
	*/
	int _maPimItemAddValue(int item, int field, int bufPointer, int bufSize, int attributes, JNIEnv* jNIEnv, jobject jThis);

	/** Removes a value from a field and item.
	* \param item Opened by maPimListNext().
	* \param field One of the \link #MA_PIM_FIELD_CONTACT_ADDR MA_PIM_FIELD \endlink constants.
	* \param index \>= 0 and \< maPimItemFieldCount().
	* \returns 0.
	* \note The changes is not actually written to disk until
	* the item is closed with maPimItemClose(). If the program exits before then,
	* the changes are lost.
	*/
	int _maPimItemRemoveValue(MAHandle item, int field, int index, JNIEnv* jNIEnv, jobject jThis);

	/** Closes the handle to a PimItem, releasing resources used by the system.
	* Writes any changes to disk.
	* Panics on invalid handles.
	* Returns 0.
	* \note An item can also be closed by maPimItemRemove().
	* \warning Does not write changes to disk if the item's List is closed.
	* Make sure to keep the List open while editing items.
	*/
	int _maPimItemClose(MAHandle item, JNIEnv* jNIEnv, jobject jThis);

	/** Creates a new empty item in the specified list.
	* \returns A handle to the new item, or \< 0 on error.
	* \note The handle must be closed with maPimItemClose() to avoid memory leaks.
	*/
	MAHandle _maPimItemCreate(MAHandle list, JNIEnv* jNIEnv, jobject jThis);

	/** Removes an item from a list. This also has the effect of closing the item.
	* \returns 0.
	* \see maPimItemClose()
	*/
	int _maPimItemRemove(MAHandle list, MAHandle item, JNIEnv* jNIEnv, jobject jThis);

	int _maCameraStart(JNIEnv* jNIEnv, jobject jThis);

	int _maCameraStop(JNIEnv* jNIEnv, jobject jThis);

	int _maCameraSetPreview(MAHandle widgetHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maCameraSelect(MAHandle cameraHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maCameraNumber(JNIEnv* jNIEnv, jobject jThis);

	int _maCameraSnapshot(int formatIndex, MAHandle placeHolder, JNIEnv* jNIEnv, jobject jThis);

	int _maCameraSnapshotAsync(int dataPlaceholder, int sizeIndex, JNIEnv* jNIEnv, jobject jThis);

	int _maCameraRecord(int flag, JNIEnv* jNIEnv, jobject jThis);

	int _maCameraFormatNumber(JNIEnv* jNIEnv, jobject jThis);

	int _maCameraFormat(int index, int sizeInfoBuffer, JNIEnv* jNIEnv, jobject jThis);

	int _maCameraSetProperty(const char *property, const char* value,
							JNIEnv* jNIEnv, jobject jThis);

	int _maCameraGetProperty(int memStart, const char *property,
								int memBuffer, int memBufferSize,
								JNIEnv* jNIEnv, jobject jThis);

	int _maCameraPreviewSize(JNIEnv* jNIEnv, jobject jThis);

	int _maCameraPreviewEventEnable(int memStart, int previewEventType,
									int previewBuffer,
									MARect* previewArea,
									JNIEnv* jNIEnv, jobject jThis);

	int _maCameraPreviewEventDisable(JNIEnv* jNIEnv, jobject jThis);

	int _maCameraPreviewEventConsumed(JNIEnv* jNIEnv, jobject jThis);

	MAExtensionModule _maExtensionModuleLoad(JNIEnv* jNIEnv, jobject jThis, const char* name, int hash);

	MAExtensionFunction _maExtensionFunctionLoad(JNIEnv* jNIEnv, jobject jThis, MAHandle module, int index);

	int _maExtensionFunctionInvoke2(JNIEnv* jNIEnv, jobject jThis, MAExtensionFunction fn, int numargs, int ptrs, int memStart);

	int _maNFCStart(JNIEnv* jNIEnv, jobject jThis);

	int _maNFCStop(JNIEnv* jNIEnv, jobject jThis);

	MAHandle _maNFCReadTag(MAHandle nfcContext, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCDestroyTag(MAHandle tagHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCConnectTag(MAHandle tagHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCCloseTag(MAHandle tagHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCIsType(MAHandle tagHandle, int tagType, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetTypedTag(MAHandle tagHandle, int tagType, JNIEnv* jNIEnv, jobject jThis);

	MAHandle _maNFCBatchStart(MAHandle nfcContext, JNIEnv* jNIEnv, jobject jThis);

	MAHandle _maNFCBatchCommit(MAHandle nfcContext, JNIEnv* jNIEnv, jobject jThis);

	MAHandle _maNFCBatchRollback(MAHandle nfcContext, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCTransceive(MAHandle tagHandle, int src, int srcLen, int dst, int dstLen, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetSize(MAHandle tagHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetId(MAHandle tagHandle, int dst, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCReadNDEFMessage(MAHandle tag, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCWriteNDEFMessage(MAHandle tag, MAHandle ndefMessage, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCCreateNDEFMessage(int recordCount, JNIEnv* jNIEnv, jobject jThis);

	MAHandle _maNFCGetNDEFMessage(MAHandle tag, JNIEnv* jNIEnv, jobject jThis);

	MAHandle _maNFCGetNDEFRecord(MAHandle ndef, int ix, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetNDEFRecordCount(MAHandle ndef, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetNDEFId(MAHandle ndefRecord, int dst, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetNDEFPayload(MAHandle ndefRecord, int dst, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetNDEFTnf(MAHandle ndefRecord, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetNDEFType(MAHandle ndefRecord, int dst, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCSetNDEFId(MAHandle ndefRecord, int src, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCSetNDEFPayload(MAHandle ndefRecord, int src, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCSetNDEFTnf(MAHandle ndefRecord, int tnf, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCSetNDEFType(MAHandle ndefRecord, int src, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCAuthenticateMifareSector(MAHandle mfcTag, int keyType, int sectorIndex, int keyAddr, int keyLen, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetMifareSectorCount(MAHandle mfcTag, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCGetMifareBlockCountInSector(MAHandle mfcTag, int sectorIndex, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCMifareSectorToBlock(MAHandle mfcTag, int sectorIndex, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCReadMifareBlocks(MAHandle mfcTag, int firstBlock, int dst, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCReadMifarePages(MAHandle mfcTag, int firstPage, int dst, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCWriteMifareBlocks(MAHandle mfcTag, int firstBlock, int src, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCWriteMifarePages(MAHandle mfcTag, int firstPage, int src, int len, int memStart, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCSetReadOnly(MAHandle tag, JNIEnv* jNIEnv, jobject jThis);

	int _maNFCIsReadOnly(MAHandle tag, JNIEnv* jNIEnv, jobject jThis);

	// ********** ADS API **********

	int _maAdsBannerCreate(int bannerSize, const char* publisherID, JNIEnv* jNIEnv, jobject jThis);

	int _maAdsAddBannerToLayout(MAHandle bannerHandle, MAHandle layoutHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maAdsRemoveBannerFromLayout(MAHandle bannerHandle, MAHandle layoutHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maAdsBannerDestroy(MAHandle bannerHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maAdsBannerSetProperty(MAHandle bannerHandle, const char* property, const char* value, JNIEnv* jNIEnv, jobject jThis);

	int _maAdsBannerGetProperty(int memStart, MAHandle bannerHandle, const char* property, int memBuffer, int bufSize, JNIEnv* jNIEnv, jobject jThis);

	// ********** Notifications API **********

	int _maNotificationLocalCreate(JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationLocalDestroy(MAHandle notificationHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationLocalSetProperty(MAHandle notificationHandle, const char* property, const char* value, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationLocalGetProperty(int memStart, MAHandle notificationHandle, const char* property, int memBuffer, int bufSize, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationLocalSchedule(MAHandle notificationHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationLocalUnschedule(MAHandle notificationHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationPushRegister(MAHandle pushNotificationTypes, const char* accountID, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationPushGetRegistration(int memStart, int messagePointer, int messageSize, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationPushUnregister(JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationPushGetData(MAHandle pushNotificationHandle, int memStart, int buffer, int messageSize, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationPushDestroy(MAHandle pushNotificationHandle, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationPushSetTickerText(const char* text, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationPushSetMessageTitle(const char* text, JNIEnv* jNIEnv, jobject jThis);

	int _maNotificationPushSetDisplayFlag(int flag, JNIEnv* jNIEnv, jobject jThis);

	int _maSyscallPanicsEnable(JNIEnv* jNIEnv, jobject jThis);

	int _maSyscallPanicsDisable(JNIEnv* jNIEnv, jobject jThis);

	int _maGetCellInfo(MAHandle mem, int memStart, JNIEnv* jNIEnv, jobject jThis);

	// ********** Capture API  **********

	int _maCaptureSetProperty(const char* property, const char* value, JNIEnv* jNIEnv, jobject jThis);

	int _maCaptureGetProperty(int memStart, const char* property, int memBuffer, int bufSize, JNIEnv* jNIEnv, jobject jThis);

	int _maCaptureAction(int action, JNIEnv* jNIEnv, jobject jThis);

	int _maCaptureWriteImage(MAHandle handle, const char* fullPathBuffer, int fullPathBufSize, JNIEnv* jNIEnv, jobject jThis);

	int _maCaptureGetImagePath(int memStart, MAHandle handle, int buffer, int bufferSize, JNIEnv* jNIEnv, jobject jThis);

	int _maCaptureGetVideoPath(int memStart, MAHandle handle, int buffer, int bufferSize, JNIEnv* jNIEnv, jobject jThis);

	int _maCaptureDestroyData(MAHandle handle, JNIEnv* jNIEnv, jobject jThis);

	// ********** In-APP Purchase API **********

	int _maPurchaseSupported(JNIEnv* jNIEnv, jobject jThis);

	int _maPurchaseCreate(MAHandle handle, const char* productID, JNIEnv* jNIEnv, jobject jThis);

	int _maPurchaseSetPublicKey(const char* developerKey, JNIEnv* jNIEnv, jobject jThis);

	int _maPurchaseRequest(MAHandle productHandle, int quantity, JNIEnv* jNIEnv, jobject jThis);

	int _maPurchaseGetField(int memStart, MAHandle productHandle, const char* property,
							int buffer, int bufferSize,
							JNIEnv* jNIEnv, jobject jThis);

	int _maPurchaseGetName(int memStart, MAHandle productHandle,
						  int buffer, int bufferSize,
						  JNIEnv* jNIEnv, jobject jThis);

	int _maPurchaseVerifyReceipt(MAHandle handle, JNIEnv* jNIEnv, jobject jThis);

	int _maPurchaseRestoreTransactions(JNIEnv* jNIEnv, jobject jThis);

	int _maPurchaseDestroy(MAHandle handle, JNIEnv* jNIEnv, jobject jThis);

	// ********** Database API **********

	/**
	 * Open a database, the database is created if it does not exist.
	 * @param path Full path to database file name.
	 * @return Handle to the database >0 on success, #MA_DB_ERROR on error.
	 */
	int _maDBOpen(const char* path, JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Close a database.
	 * @param databaseHandle Handle to the database.
	 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
	 */
	int _maDBClose(MAHandle databaseHandle, JNIEnv* jNIEnv, jobject jThis);

	/**
	 * Executes an SQL statement. If the statement returns a
	 * query result, a cursor handle is returned.
	 * @param databaseHandle Handle to the database.
	 * @param sql The SQL statement.
	 * @return #MA_DB_ERROR on error, #MA_DB_OK on success,
	 * > 0 if there is a cursor to a query result, in this
	 * case the return value is the cursor handle.
	 */
	int _maDBExecSQL(
		MAHandle databaseHandle,
		const char* sql,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Invoke maDBExecSQLParams.
	 */
	int _maDBExecSQLParams(
		MAHandle databaseHandle,
		const char* sql,
		int paramsAddress,
		int paramCount,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Destroys a cursor. You must call this function
	 * when you are done with the cursor to release
	 * its resources.
	 * @param cursorHandle Handle to the cursor.
	 */
	int _maDBCursorDestroy(
		MAHandle cursorHandle,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Move the cursor to the next row in the result set.
	 * Note that you must call this function before retrieving
	 * column data. The initial position of the cursor is
	 * before the first row in the result set. If the result
	 * set is empty, this function will return a value != MA_DB_OK.
	 * @param cursorHandle Handle to the cursor.
	 * @return #MA_DB_OK if successfully moved to next row,
	 * #MA_DB_NO_ROW if there are no more rows in the result set,
	 * #MA_DB_ERROR on error.
	 */
	int _maDBCursorNext(
		MAHandle cursorHandle,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor as a data object. Use this function for
	 * blob data or text data.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param placeholder Handle created with maCreatePlaceholder.
	 * A data object will be created with the column data, and the handle
	 * will refer to that data.
	 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
	 */
	int _maDBCursorGetColumnData(
		MAHandle cursorHandle,
		int columnIndex,
		MAHandle placeholder,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor as a text data buffer. Use this function for
	 * text data.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param bufferAddress Address to buffer to receive the data.
	 * The result is NOT zero terminated.
	 * @param bufferSize Max size of the buffer.
	 * @return The actual length of the data, if the actual length
	 * returned is > bufferSize, data was not copied (buffer too small),
	 * returns #MA_DB_ERROR on other errors.
	 */
	int _maDBCursorGetColumnText(
		MAHandle cursorHandle,
		int columnIndex,
		int bufferAddress,
		int bufferSize,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor as an int value.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param intValueAddress Address to int to receive the value.
	 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
	 */
	int _maDBCursorGetColumnInt(
		MAHandle cursorHandle,
		int columnIndex,
		int intValueAddress,
		JNIEnv* jNIEnv,
		jobject jThis);

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor as a double value.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param doubleValueAddress Address to double to receive the value.
	 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
	 */
	int _maDBCursorGetColumnDouble(
		MAHandle cursorHandle,
		int columnIndex,
		int doubleValueAddress,
		JNIEnv* jNIEnv,
		jobject jThis);


	// AUDIO API

	//MAAudioData maAudioDataCreateFromResource(in MAString mime, in MAHandle data, in int offset, in int length, in int flags);
	int _maAudioDataCreateFromResource(
		const char* mime,
		int data,
		int offset,
		int length,
		int flags,
		JNIEnv* jNIEnv,
		jobject jThis);

	//MAAudioData maAudioDataCreateFromURL(in MAString mime, in MAString url, in int flags);
	int _maAudioDataCreateFromURL(
		const char* mime,
		const char* url,
		int flags,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioDataDestroy(in MAAudioData audioData);
	int _maAudioDataDestroy(
		int audioData,
		JNIEnv* jNIEnv,
		jobject jThis);

	//MAAudioInstance maAudioInstanceCreate(in MAAudioData audioData);
	int _maAudioInstanceCreate(
		int audioData,
		JNIEnv* jNIEnv,
		jobject jThis);

	int _maAudioInstanceCreateDynamic(
		int sampleRate,
		int numChannels,
		int bufferSize,
		JNIEnv* jNIEnv,
		jobject jThis);

	int _maAudioSubmitBuffer(
		int audioInstance,
		int buffer,
		int bufferSize,
		int memStart,
		JNIEnv* jNIEnv,
		jobject jThis);

	int _maAudioGetPendingBufferCount(
		int audioInstance,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioInstanceDestroy(in MAAudioInstance audioInstance);
	int _maAudioInstanceDestroy(
		int audioInstance,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioGetLength(in MAAudioInstance audio);
	int _maAudioGetLength(
		int audio,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioSetNumberOfLoops(in MAAudioInstance audio, in int loops);
	int _maAudioSetNumberOfLoops(
		int audio,
		int loops,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioPrepare(in MAAudioInstance audio, in int async);
	int _maAudioPrepare(
		int audio,
		int async,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioPlay(in MAAudioInstance audio);
	int _maAudioPlay(
		int audio,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioSetPosition(in MAAudioInstance audio, in int milliseconds);
	int _maAudioSetPosition(
		int audio,
		int milliseconds,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioGetPosition(in MAAudioInstance audio);
	int _maAudioGetPosition(
		int audio,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioSetVolume(in MAAudioInstance audio, in float volume);
	int _maAudioSetVolume(
		int audio,
		float volume,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioPause(in MAAudioInstance audio);
	int _maAudioPause(
		int audio,
		JNIEnv* jNIEnv,
		jobject jThis);

	//int maAudioStop(in MAAudioInstance audio);
	int _maAudioStop(
		int audio,
		JNIEnv* jNIEnv,
		jobject jThis);
}

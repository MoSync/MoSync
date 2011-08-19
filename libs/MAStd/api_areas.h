/** \file api_areas.h
* \brief Overview of the MoSync API
* \author Fredrik Eldh
*/

/** \mainpage MoSync API Reference
<center>The essential guide to MoSync syscalls and libraries</center>
<br>

<table style="border-color: lightgrey;">
<tr style="background-color: rgb(53, 28, 117);">
<td colspan="2">
<span style="color: rgb(255, 255, 255);"><strong>MoSync-Specific Functions</strong></span>


</td>

<td>
<span style="color: rgb(255, 255, 255);"><strong>Syscalls (maapi.h)</strong></span>
</td>
<td>
<span style="color: rgb(255, 255, 255);"><strong>Libraries</strong></span>
</td>
<td>
<strong><span style="color: rgb(255, 255, 255);">See also</span><br>
</strong>
</td>
</tr>

<tr style="background-color: rgb(217, 210, 233);">
<td>
<strong>System functions</strong>
</td>
<td>Exit application, report non-recoverable error.
</td>
<td>
maExit(), maPanic()
</td>
<td>
&nbsp;
</td>
<td>
&nbsp;
</td>
</tr>

<tr style="background-color: rgb(217, 210, 233);">
<td>
<strong><a name="event"></a>Event handling</strong>
</td>
<td>
User input, communications, asynchronous events.
</td>
<td>
maWait(), maGetEvent()
</td>
<td>
MAUtil::Environment, MAUtil::Moblet, \ref MAUtil/CharInput.h
</td>
<td>
&nbsp;
</td>
</tr>

<tr style="background-color: rgb(217, 210, 233);">
<td>
<strong><a name="res"></a>Resource&nbsp;management</strong>
</td>
<td>
Create, read, write binary objects, destroy objects, create resource handles.
</td>
<td>
maCreateData(), maCreatePlaceholder(), maDestroyObject(), maGetDataSize(), maReadData(),<br>
maWriteData()
</td>
<td>
MAUtil::DataHandler, MAUtil::PlaceholderPool
</td>
<td>
&nbsp;
</td>
</tr>

<tr style="background-color: rgb(217, 210, 233);">
<td>
<strong>Dynamic loading</strong>
</td>
<td>Load code and data from binary object.<br>
</td>
<td>maLoadResources(), maLoadProgram()<br>
</td>
<td><br>
</td>
<td><a href="#res" class="el">Resource management</a><br>
</td>
</tr>
<tr style="background-color: rgb(11, 147, 148);">
<td colspan="2">
<span style="color: rgb(255, 255, 255);"><strong>Functions with MoSync interfaces</strong></span>


</td>

<td>
<span style="color: rgb(255, 255, 255);"><strong>Syscalls (maapi.h)</strong></span>
</td>
<td>
<span style="color: rgb(255, 255, 255);"><strong>Libraries</strong></span>
</td>
<td>
<strong><span style="color: rgb(255, 255, 255);">See also</span><br>
</strong>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td>
<strong>Permanent storage</strong>
</td>
<td>Store settings, values, downloaded data and programs.<br>
</td>
<td>maOpenStore(), maWriteStore(), maReadStore(), maCloseStore()<br>
</td>
<td><br>
</td>
<td><a href="#res" class="el">Resource management</a><br>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td>
<strong>Graphics</strong>
</td>
<td>Draw pixels, lines, rectangles, triangles and images to either the screen or to a drawable image. Create new images, either empty or from compressed or uncompressed data. Use a clipping rectange to control drawing.<br>
</td>
<td>maSetColor(), maPlot(), maLine(), maFillRect(), maFillTriangleStrip(), maFillTriangleFan(), maDrawImage(), maDrawText(), maDrawTextW(), maGetTextSize(), maGetTextSizeW(), maUpdateScreen(), maResetBacklight(), maGetScrSize(), maDrawRGB(), maDrawImageRegion(), maGetImageSize(), maGetImageData(), maSetDrawTarget(), maSetClipRect(), maGetClipRect(), maCreateImageFromData(), maCreateImageRaw(), maCreateDrawableImage()<br>
</td>
<td>\ref MAUtil/Graphics.h<br>
</td>
<td><a href="#res" class="el">Resource management</a><br>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td>
<strong>Framebuffer</strong>
</td>
<td>Access the device's graphical framebuffer (more or less) directly, allowing more advanced graphics operations.<br>
</td>
<td>maFrameBufferGetInfo(), maFrameBufferInit(), maFrameBufferClose()<br>
</td>
<td>\ref MAUtil/FrameBuffer.h<br>
</td>
<td><br>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td>
<strong>Sound</strong>
</td>
<td>Play a sound, control volume.<br>
</td>
<td>maSoundPlay(), maSoundStop(), maSoundIsPlaying(), maSoundGetVolume(), maSoundSetVolume()<br>
</td>
<td><br>
</td>
<td><a href="#res" class="el">Resource management</a><br>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td>
<strong>Time and date</strong>
</td>
<td>Check current time and date, convert to C string.<br>
</td>
<td>maGetMilliSecondCount(), maTime(), maLocalTime()<br>
</td>
<td>matime.h<br>
</td>
<td><br>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td><a name="comms"></a>
<strong>Communications</strong>
</td>
<td>Communicate via TCP, HTTP, SSL, HTTPS and Bluetooth serial port.<br>
</td>
<td>maConnect(), maConnClose(), maConnRead(), maConnWrite(), maConnReadToData(), maConnWriteFromData(), maHttpCreate(), maHttpSetRequestHeader(), maHttpGetResponseHeader(), maHttpFinish()<br>
</td>
<td>\ref MAUtil/Connection.h, \ref MAUtil/BluetoothConnection.h, \ref MAUtil/Downloader.h, \ref MAUtil/BuffDownloader.h<br>
</td>
<td><a href="#event" class="el">Event handling</a>,
<a href="#res" class="el">Resource management</a>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td>
<strong>Servers</strong>
</td>
<td>Advertise services, listen for incoming connections.<br>
</td>
<td>maConnect(), maConnClose(), maAccept()<br>
</td>
<td>\ref MAUtil/Server.h<br>
</td>
<td><a href="#comms" class="el">Communications</a>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td>
<strong>Bluetooth discovery</strong>
</td>
<td>Search for nearby Bluetooth devices and query their capabilities.<br>
</td>
<td>maBtStartDeviceDiscovery(), maBtGetNewDevice(), maBtStartServiceDiscovery(), maBtGetNextServiceSize(), maBtGetNewService(), maBtCancelDiscovery()<br>
</td>
<td>\ref MAUtil/BluetoothDiscovery.h, \ref MAUtil/mauuid.h<br>
</td>
<td><a href="#event" class="el">Event handling</a><br>
</td>
</tr>

<tr style="background-color: rgb(207, 226, 243);">
<td><strong>Location</strong><br>
</td>
<td>Read your current geographical location.<br>
</td>
<td>maLocationStart(), maLocationStop()<br>
</td>
<td><br>
</td>
<td><a href="#event" class="el">Event handling</a><br>
</td>
</tr>
<tr style="background-color: rgb(207, 226, 243);">
<td><strong>Filesystem</strong><br>
</td>
<td>Access the device's filesystem; list, create and delete files and directories; open, close, read and write files.<br>
</td>
<td>maFileListStart(), maFileListNext(), maFileListClose(), maFileOpen(), maFileExists(), maFileClose(), maFileCreate(), maFileDelete(), maFileSize(), maFileAvailableSpace(), maFileTotalSpace(), maFileDate(), maFileRename(), maFileTruncate(), maFileWrite(), maFileWriteFromData(), maFileRead(), maFileReadToData(), maFileTell(), maFileSeek()<br>
</td>
<td><br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(207, 226, 243);">
<td><strong>Camera</strong><br>
</td>
<td>Access the available functionality on device's camera to capture still images.<br>
</td>
<td>maCameraSelect(), maCameraNumber(), maCameraSetProperty(), maCameraGetProperty(), maCameraSetPreview(), maCameraFormatNumber(), maCameraFormat(), maCameraStart(), maCameraStop(), maCameraSnapshot()<br>
</td>
<td><br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(207, 226, 243);">
<td>
<strong>Miscellaneous functions</strong>
</td>
<td>Detect key state, control vibration, measure battery, platform request, send SMS, switch focus, get system property, lock/unlock keypad.<br>
</td>
<td>maGetKeys(), maVibrate(), maGetBatteryCharge(), maPlatformRequest(), maSendTextSMS(), maSendToBackground(), maBringToForeground(), maGetSystemProperty(), maLockKeypad(), maUnlockKeypad(), maKeypadIsLocked()<br>
</td>
<td>\ref maassert.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(118, 29, 88);">
<td colspan="2"><strong><span style="color: rgb(255, 255, 255);">User Interface APIs</span></strong><strong><br>
</strong></td>

<td><strong><span style="color: rgb(255, 255, 255); a:link { color: white; }">Syscalls (maapi.h)</span><br>
</strong></td>
<td><strong><span style="color: rgb(255, 255, 255);">Libraries</span><br>
</strong></td>
<td><strong><span style="color: rgb(255, 255, 255);">See also</span><br>
</strong></td>
</tr>
<tr style="background-color: rgb(235, 211, 227);">
<td>
<strong>Native user interface</strong>
</td>
<td>Access to device specific UI system. Currently only available for Android and iOS.<br>
</td>
<td>maWidgetCreate(), maWidgetDestroy(), maWidgetAddChild(), maWidgetInsertChild(), maWidgetRemoveChild(), maWidgetScreenShow(), maWidgetStackScreenPush(), maWidgetStackScreenPop(), maWidgetSetProperty(), maWidgetGetProperty()<br>
</td>
<td>IX_WIDGET.h<br>
</td>
<td>\ref WidgetAPI "Widget API"<br>
</td>
</tr>
<tr style="background-color: rgb(235, 211, 227);">
<td>
<strong>Platform independent user interface</strong>
</td>
<td>Image, animated image, edit box, list box, screens, widgets, widget skins, input managers, etc.<br>
</td>
<td>maTextBox()<br>
</td>
<td>MAUI::EditBox, MAUI::Engine, MAUI::Font, MAUI::Image, MAUI::Label, MAUI::Layout, MAUI::ListBox, MAUI::Screen, MAUI::Widget<br>
</td>
<td>MAUI<br>
</td>
</tr>
<tr style="background-color: rgb(56, 118, 29);">
<td colspan="2"><strong><span style="color: rgb(255, 255, 255);">Standard C Functions</span></strong><strong><br>
</strong></td>

<td><strong><span style="color: rgb(255, 255, 255);">Syscalls (maapi.h)</span><br>
</strong></td>
<td><strong><span style="color: rgb(255, 255, 255);">Libraries</span><br>
</strong></td>
<td><strong><span style="color: rgb(255, 255, 255);">See also</span><br>
</strong></td>
</tr>
<tr style="background-color: rgb(217, 234, 211);">
<td>
<strong>Math</strong>
</td>
<td>Trigonometric functions, common constants, etc.<br>
</td>
<td>sin(), cos(), tan(), sqrt()<br>
</td>
<td>limits.h, madmath.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(217, 234, 211);">
<td>
<strong>Memory<br>
</strong>
</td>
<td>Allocate and free memory. Copy and set data.<br>
</td>
<td>memset(), memcpy()<br>
</td>
<td>maheap.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(217, 234, 211);">
<td>
<strong>C string handling<br>
</strong>
</td>
<td>Compare, copy, format and convert null-terminated 8-bit strings.<br>
</td>
<td>strcmp(), strcpy()<br>
</td>
<td>mactype.h, mastring.h, maxtoa.h, mavsprintf.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(217, 234, 211);">
<td><strong>Wide-char string handling</strong><br>
</td>
<td>Compare, copy, format and convert null-terminated 16-bit strings.<br>
</td>
<td><br>
</td>
<td>wchar.h, mawstring.h, mawvsprintf.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(217, 234, 211);">
<td>
<strong>Miscellaneous</strong>
</td>
<td>Variadric function helpers and random number generation.<br>
</td>
<td><br>
</td>
<td>maarg.h, mastdlib.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(217, 234, 211);">
<td>
<strong>Console output<br>
</strong>
</td>
<td>Print text to the screen, with colour and logging options.<br>
</td>
<td><br>
</td>
<td>conprint.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(217, 234, 211);">
<td>
<strong>Virtual file system</strong>
</td>
<td>A subset of the buffered file I/O library.<br>
</td>
<td><br>
</td>
<td>\ref MAFS/File.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(180, 95, 6);">
<td colspan="2"><strong><span style="color: rgb(255, 255, 255);">Miscellaneous libraries</span></strong><strong><br>
</strong></td>

<td><strong><span style="color: rgb(255, 255, 255);">Syscalls (maapi.h)</span><br>
</strong></td>
<td><strong><span style="color: rgb(255, 255, 255);">Libraries</span><br>
</strong></td>
<td><strong><span style="color: rgb(255, 255, 255);">See also</span><br>
</strong></td>
</tr>
<tr style="background-color: rgb(249, 203, 156);">
<td>
<strong>C++ utilities</strong>
</td>
<td>Containers, geomeric helper structures, utilities.<br>
</td>
<td><br>
</td>
<td>MAUtil::Vector, MAUtil::String, MAUtil::HashMap, MAUtil::Map, MAUtil::Set, MAUtil::List,<br>
\ref MAUtil/Geometry.h, \ref MAUtil/util.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(249, 203, 156);">
<td>
<strong>XML processing</strong>
</td>
<td>Tiny XML parser.<br>
</td>
<td><br>
</td>
<td>\ref MTXml/MTXml.h<br>
</td>
<td><br>
</td>
</tr>
<tr style="background-color: rgb(249, 203, 156);">
<td><strong>Slippy maps</strong><br>
</td>
<td>Download, cache and display maps from multiple sources in different resolutions.<br>
</td>
<td><br>
</td>
<td>MAP<br>
</td>
<td><br>
</td>
</tr>
</table>

*/

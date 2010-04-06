/** \page api_areas API areas
You can do many things with MoSync, and some things can be done in several different
ways. This document will give you an overview of all these things.
<br>
<br>
<br>
\section mosync MoSync-specific functions


\subsection sys System functions

Syscalls: maExit(), maPanic().<br>

\subsection event Event handling

Recieve information about user input, communications and other asynchronous events.<br>
Syscalls: maWait(), maGetEvent().<br>
Libs: MAUtil::Environment, MAUtil::Moblet, \ref MAUtil/CharInput.h<br>

\subsection res Resource management

Create, read and write large binary objects, destroy any object, create new resource handles.<br>
Syscalls: maCreateData(), maCreatePlaceholder(), maDestroyObject(), maGetDataSize(), maReadData(),
maWriteData().<br>
Libs: MAUtil::DataHandler, MAUtil::PlaceholderPool<br>

\subsection dyna Dynamic loading

Load code and data from a binary object, which can be filled with anything from
anywhere.<br>
Syscalls: maLoadResources(), maLoadProgram().<br>
Uses \ref res.
<br>
<br>
<br>
\section common Common functions with a MoSync-specific interface

\subsection rms Permanent storage

Useful for storing settings, high-scores, downloaded data and programs.<br>
Syscalls: maOpenStore(), maWriteStore(), maReadStore(), maCloseStore().<br>
Uses \ref res.

\subsection Graphics

Draw pixels, lines, rectangles, triangles and images to either the screen or to
a drawable image. Create new images, either empty or from compressed or uncompressed data.
Use a clipping rectange to control drawing.<br>
Syscalls: maSetColor(), maPlot(), maLine(), maFillRect(), maFillTriangleStrip(),
maFillTriangleFan(), maDrawImage(),
maDrawText(), maDrawTextW(), maGetTextSize(), maGetTextSizeW(), maUpdateScreen(),
maResetBacklight(), maGetScrSize(), maDrawRGB(),
maDrawImageRegion(), maGetImageSize(), maGetImageData(), maSetDrawTarget(), maSetClipRect(),
maGetClipRect(), maCreateImageFromData(), maCreateImageRaw(), maCreateDrawableImage().<br>
Libs: \ref MAUtil/Graphics.h<br>
See also: \ref res.<br>

\subsubsection Framebuffer

Access the device's graphical framebuffer (more or less) directly.
This allows for more advanced graphics operations that the regular API.<br>
Syscalls: maFrameBufferGetInfo(), maFrameBufferInit(), maFrameBufferClose().<br>
Libs: \ref MAUtil/FrameBuffer.h

\subsection Sound

Play a sound, with volume control.<br>
Syscalls: maSoundPlay(), maSoundStop(), maSoundIsPlaying(), maSoundGetVolume(),
maSoundSetVolume().<br>
See also: \ref res.<br>

\subsection time Time and date

Check the current time and date, convert to C string.<br>
Syscalls: maGetMilliSecondCount(), maTime(), maLocalTime().<br>
Libs: matime.h<br>

\subsection Communications

Speak to other machines using TCP, HTTP or a Bluetooth serial port.<br>
Syscalls: maConnect(), maConnClose(), maConnRead(), maConnWrite(), maConnReadToData(),
maConnWriteFromData(), maHttpCreate(), maHttpSetRequestHeader(), maHttpGetResponseHeader(),
maHttpFinish().<br>
Libs: \ref MAUtil/Connection.h, \ref MAUtil/Downloader.h, \ref MAUtil/BuffDownloader.h<br>
Uses \ref event.<br>
See also: \ref res.<br>

\subsection bt Bluetooth discovery

Search for Bluetooth devices in the vicinity and ask them what services they provide.<br>
Syscalls: maBtStartDeviceDiscovery(), maBtGetNewDevice(), maBtStartServiceDiscovery(),
maBtGetNextServiceSize(), maBtGetNewService(), maBtCancelDiscovery().<br>
Uses \ref event.<br>
Libs: \ref MAUtil/BluetoothDiscovery.h, \ref MAUtil/mauuid.h<br>

\subsection Location

Read your current geographical location.<br>
Syscalls: maLocationStart(), maLocationStop().<br>
Uses \ref event.<br>

\subsection Filesystem

Access the device's filesystem. List, create and delete files and directories.
Open, close, read and write files.<br>
Syscalls: maFileListStart(), maFileListNext(), maFileListClose(),
maFileOpen(), maFileExists(), maFileClose(), maFileCreate(), maFileDelete(),
maFileSize(), maFileAvailableSpace(), maFileTotalSpace(), maFileDate(), maFileRename(),
maFileTruncate(), maFileWrite(), maFileWriteFromData(), maFileRead(), maFileReadToData(),
maFileTell(), maFileSeek().<br>

\subsection Camera

Use the device's camera to display a viewfinder and capture snapshots.<br>
Syscalls: maCameraFormatNumber(), maCameraFormat(), maCameraStart(), maCameraStop(),
maCameraSnapshot().<br>

\subsection sysmisc Miscellaneous functions

Syscalls: maGetKeys(), maVibrate(), maGetBatteryCharge(), maPlatformRequest(), maSendTextSMS(),
maSendToBackground(), maBringToForeground(), maGetSystemProperty(),
maLockKeypad(), maUnlockKeypad(), maKeypadIsLocked().<br>
Libs: \ref maassert.h<br>
<br>
<br>
<br>


\section std Standard C functions

\subsection Math

Trigonometric functions, common constants, and others.<br>
Syscalls: sin(), cos(), tan(), sqrt().<br>
Libs: limits.h, madmath.h.<br>

\subsection mem Memory management

Allocate and free memory. Copy and set data.<br>
Syscalls: memset(), memcpy().<br>
Libs: maheap.h<br>

\subsection c_str C string handling

Compare, copy, format and convert null-terminated 8-bit strings.<br>
Syscalls: strcmp(), strcpy().<br>
Libs: mactype.h, mastring.h, maxtoa.h, mavsprintf.h<br>

\subsection wcs Wide-char string handling

Compare, copy, format and convert null-terminated 16-bit strings.<br>
Libs: wchar.h, mawstring.h, mawvsprintf.h<br>

\subsection stdlib Miscellaneous

Among them are variadric function helpers and random number generation.<br>
Libs: maarg.h, mastdlib.h<br>

\subsection conprint Simple output console

A way to print text to the screen. Options include color and logging.<br>
Libs: conprint.h<br>

\subsection mafs Virtual file system

A subset of the buffered file I/O library.<br>
Libs: \ref MAFS/File.h<br><br>
<br>
<br>


\section misclib Miscellaneous libraries

\subsection cpp C++ utilities

Libs: MAUtil::Vector, MAUtil::String, MAUtil::HashMap, MAUtil::Map, MAUtil::Set, MAUtil::List,
\ref MAUtil/Geometry.h, \ref MAUtil/util.h<br>

\subsection xml XML processing

Libs: \ref MTXml/MTXml.h<br>
<br>
<br>
<br>


\section gui Graphical user interface

See MAUI.<br>


\subsection map Roadmaps

Download, cache and display maps of Earth. They are divided into tiles.
They are available from multiple sources and in multiple resolutions.<br>
Libs: MAP.<br>

*/

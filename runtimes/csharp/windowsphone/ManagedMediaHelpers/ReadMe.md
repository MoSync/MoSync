# ManagedMediaHelpers #

## Code Quick Start ##

If you just want to get right into the code, check out these directories

*    MediaParsers.Desktop
*    Mp3MediaStreamSource.SL4

Otherwise, read on for a little more detail.

## Overview ##

The ManagedMediaHelpers projects are a set of classes for working with
and learning about [Silverlight]'s [MediaStreamSource] which allows
media developers to ignore the media container and transport mechanism. 

You may wonder -- why would anyone want to do this?

For one thing, having access to elementary streams means that developers
can now implement scenarios that other solutions haven't necessarily
provided thus far. One example of this is adaptive streaming or
multi-bitrate support as was seen during the 2008 Olympics and which is
available with [IIS]'s [SmoothStreaming]

For another reason, having access to elementary streams allows
developers to implement scenarios that the Silverlight runtime hasn't
had a chance to implement yet or that the runtime might not be able to
implement in the same timeframe that a developer wants it. Examples of
this could be, RTSP:T protocol support, SHOUTcast protocol support,
seamless audio looping, ID3 v1 and ID3 v2 metadata support, and many
other scenarios.

Finally, having access to an object which takes elementary data (video
frames and audio samples) gives developers a chance to explore and learn
about some of the upper layers of media technologies without worrying
about setting up lots of underlying scaffolding and the headaches and
context that can entail.

## How is this project structured ##

This solution consists of 9 projects with varying extensions.

You can look at these from a platform centric POV . . . 

*    __.Desktop__ builds against .NET Framework
*    __.Phone__ builds against Windows Phone 7 
*    __.SL4__ builds against Silverlight

. . . or a purpose centric POV.

*    __Demo__ live apps for WP7 or Silverlight showing the MediaStreamSource in action
*    __Test__ test project for the MediaParsers 
*    __MediaParsers__ Utility classes, common media classes, and mp3 specific data conversion
*    __Mp3MediaStreamSource__ The core class that handles reading a media stream and parsing it as Mp3 content

## Tools used to create the project ##
*    [Visual Studio 2010]
*    [Style Cop]
*    [Windows Phone 7 Tools] 
*    [git]

## License ##

This code is released under the Microsoft Public License (Ms-PL).  See
License.txt for a complete copy of the license.

(c) Copyright 2008 Larry Olson 

Changes to support duration and streaming (i.e. non-seekable) content  
(c) Copyright 2010 Rdio.

[Visual Studio 2010]: http://msdn.microsoft.com/en-us/vstudio/default.aspx
[Style Cop]: http://stylecop.codeplex.com/
[Windows Phone 7 Tools]: http://create.msdn.com/en-us/home/getting_started
[Silverlight]: http://www.silverlight.net
[MediaStreamSource]: http://msdn.microsoft.com/en-us/library/system.windows.media.mediastreamsource(VS.95).aspx
[IIS]: http://www.iis.net/ 
[SmoothStreaming]: http://www.iis.net/download/SmoothStreaming 
[git]: http://git-scm.com/

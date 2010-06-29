MakeSIS.exe -- SDN Release, 28 April 2003  
Makesis is the tool used by Symbian to make software installation files for deployment on
target devices; more information about the Symbian build and deployment process can be found
in the new  ''Symbian OS C++ for Mobile Phones''  book (2003,by Wiley).

Makesis.exe is found in every Symbian (and product specific) SDK like the UIQ SDK,
Nokia Series 60 SDK etc. Now you can use  this source to recompile and 
augment this tool.

The Makesis tool is dependent on 4 things:

1. A Windows development set-up (this is a Windows based tool)
2. Any Symbian SDK (post v6.1) (like the one found in the above book)
3. zlib 1.1.3 (1.1.4 is not really needed since there are no security implications 
   with the tool)
4. a crypto lib (This is not provided :-)

Therefore the source has the parts that use the crypto lib #undef'ed in order not to compile.
This option makes for a perfectly valid tool and allows people to also use a crypto lib if 
they have one or want to write one (since the call signatures are in the source :-). Hence 
this release of the tool (if a crypto lib doesn't exist) is not able to handle certificates
and therefore sign the .sis files produced.

The Symbian source for the tool (in appinst\makesis directory tree) is released under the 
terms and conditions of the Symbian Example Source Code License found at:
	(http://www.symbian.com/developer/techlib/codelicense.html)
You should have received a copy of the license with this Makesis distribution package.

All other source is copyright of their respective authors.

Zlib 1.1.3 is also included in the distribution package together with some handy MSDEV dsp 
and .mak files to get you up and running quickly (CodeWarrior and other IDEs can translate 
them :-)

Zlib is Copyright (C) 1995-1998 Jean-loup Gailly and Mark Adler
Moreover for the Unicode conversions, Makesis uses some  Unicode Incs code. This code is
Copyright 2001 Unicode, Inc.

This distribution has been tested under the UIQ v2.0 SDK (as found in the book above),
Nokia Series 60 v1.0 SDK and the Symbian OS V7.0s Technology Preview SDK (as released in the
Symbian Exposium 2003).

To build Makesis you first build zlib, that is all
enjoy :-)



 
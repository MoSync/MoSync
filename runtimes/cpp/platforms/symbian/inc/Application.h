/* Copyright (C) 2009 Mobile Sorcery AB

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

#ifndef __CAPPLICATION_H__
#define __CAPPLICATION_H__

#include <Eikapp.h>

class CApplication : public CEikApplication {
	//inherited from CEikApplication
public:
	TUid AppDllUid() const;
#if defined(__WINS__) && defined(__SERIES60_3X__)
	TFileName ResourceFileName() const;
#endif	//__WINS__
protected:
	CApaDocument* CreateDocumentL();
};

#endif // __CAPPLICATION_H__

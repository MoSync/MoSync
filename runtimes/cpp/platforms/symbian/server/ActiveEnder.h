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

#ifndef ACTIVEENDER_H
#define ACTIVEENDER_H

#include <e32base.h>

class CActiveEnder;
class RMessage2;

class CActiveLink : public CActive {
public:
	CActiveLink() : CActive(CActive::EPriorityStandard) {
		CActiveScheduler::Add(this);
	}
private:
	CActiveLink* iPrev;
	friend class CActiveEnder;
};

class CActiveEnder : public CActive {
public:
	CActiveEnder();
	virtual ~CActiveEnder();

	void move(CActiveLink* aObject);

protected:	//CActive
	void RunL();
	void DoCancel();

private:
	void clean();

	CActiveLink* mLast;
};

void lazyDelete(const RMessage2& aMessage, CActiveLink* aObject);

#endif	//ACTIVEENDER_H

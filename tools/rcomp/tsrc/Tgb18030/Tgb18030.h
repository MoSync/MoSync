/*
* Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

#ifndef __TGB18030_H
#define __TGB18030_H

#include <coeccntx.h>

#include <eikenv.h>
#include <eikappui.h>
#include <eikapp.h>
#include <eikdoc.h>
#include <eikmenup.h>
#include <eikon.hrh>
#include <TXTRICH.H> 
#include <eikrted.h>
#include <TRcomp18030.rsg>
#include "Tgb18030.hrh"



////////////////////////////////////////////////////////////////////////
//
// CTgb18030Application
//
////////////////////////////////////////////////////////////////////////

class CTgb18030Application : public CEikApplication
	{
private: 
	           // Inherited from class CApaApplication
	CApaDocument* CreateDocumentL();
	TUid AppDllUid() const;
	};

////////////////////////////////////////////////////////////////////////
//
// CTgb18030AppView
//
////////////////////////////////////////////////////////////////////////
class CTgb18030AppView : public CCoeControl
    {
public:
	static CTgb18030AppView* NewL(const TRect& aRect);
	CTgb18030AppView();
	~CTgb18030AppView();
    void ConstructL(const TRect& aRect);
    void ChangeEditorText();
private:
	void SizeChanged();
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,
			TEventCode aType);
           // Inherited from CCoeControl
	void Draw(const TRect& /*aRect*/) const;

private:
	HBufC*  iTgb18030Text;
	HBufC* iRcompTestText;
	CEikRichTextEditor* iEditor;
    };


////////////////////////////////////////////////////////////////////////
//
// CTgb18030AppUi
//
////////////////////////////////////////////////////////////////////////
class CTgb18030AppUi : public CEikAppUi
    {
public:
    void ConstructL();
	~CTgb18030AppUi();

private:
              // Inherirted from class CEikAppUi
	void HandleCommandL(TInt aCommand);

private:
	CCoeControl* iAppView;
	};


////////////////////////////////////////////////////////////////////////
//
// CTgb18030Document
//
////////////////////////////////////////////////////////////////////////
class CTgb18030Document : public CEikDocument
	{
public:
	static CTgb18030Document* NewL(CEikApplication& aApp);
	CTgb18030Document(CEikApplication& aApp);
	void ConstructL();
private: 
	           // Inherited from CEikDocument
	CEikAppUi* CreateAppUiL();
	};


#endif


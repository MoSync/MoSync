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


#include "Tgb18030.h"

//             The second phase constructor of the application UI class.
//             The application UI creates and owns the one and only view.
// 
void CTgb18030AppUi::ConstructL()
    {
				// BaseConstructL() completes the UI framework's
	           // construction of the App UI.
    BaseConstructL();
	iAppView = CTgb18030AppView::NewL(ClientRect());
	}


//             The app Ui owns the two views and is. 
//             therefore, responsible for destroying them
//
CTgb18030AppUi::~CTgb18030AppUi()
	{
	delete iAppView;
	}


void CTgb18030AppUi::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		      // Just issue simple info messages to show that
		      // the menu items have been selected
	case ETgb18030Item0:
		((CTgb18030AppView*)iAppView)->ChangeEditorText();
		break;

	
	case ETgb18030Item1:
		break;
	
		
	case EEikCmdExit: 
		Exit();
		break;
		}
	}


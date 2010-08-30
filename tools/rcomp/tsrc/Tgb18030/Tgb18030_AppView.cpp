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
#include <barsread.h>
//
//             Constructor for the view.
//
CTgb18030AppView::CTgb18030AppView()
	{
	}


//             Static NewL() function to start the standard two
//             phase construction.
//
CTgb18030AppView* CTgb18030AppView::NewL(const TRect& aRect)
	{
	CTgb18030AppView* self = new(ELeave) CTgb18030AppView();
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop();
	return self;
	}


//
//             Destructor for the view.
//
CTgb18030AppView::~CTgb18030AppView()
	{
	delete iTgb18030Text;
	delete iRcompTestText;
	delete iEditor;
	}


//             Second phase construction.
//
void CTgb18030AppView::ConstructL(const TRect& aRect)
    {
			   // Fetch the text from the resource file.
	iTgb18030Text = iEikonEnv->AllocReadResourceL(R_TGB18030_TEXT_HELLO);
	iRcompTestText = iEikonEnv->AllocReadResourceL(R_TRCOMP_TEXT_POS1);	

	// Control is a window owning control
	CreateWindowL();
	           // Extent of the control. This is
	           // the whole rectangle available to application.
	           // The rectangle is passed to us from the application UI.

    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC( reader, R_OUTPUT_VIEW_RTEXTED );
    iEditor = new ( ELeave ) CEikRichTextEditor();

    iEditor->SetContainerWindowL( *this );
    iEditor->ConstructFromResourceL( reader );
    CleanupStack::PopAndDestroy();  // Resource reader
    iEditor->SetFocus( ETrue );

    iEditor->SetExtent( TPoint( 0, 0 ),
                              TSize( aRect.Width(), aRect.Height() ) );

    iEditor->CreateScrollBarFrameL();
    iEditor->ScrollBarFrame()->SetScrollBarVisibilityL(CEikScrollBarFrame::EOn, CEikScrollBarFrame::EOn);
    iEditor->ScrollBarFrame()->DrawScrollBarsNow();
    iEditor->SetTextL( iTgb18030Text );

    
    
	SetRect(aRect);
	
	iEditor->DrawNow();
			   // At this stage, the control is ready to draw so
	           // we tell the UI framework by activating it.
	ActivateL();
	}

void CTgb18030AppView::ChangeEditorText()
	{
	iEditor->SetTextL(iRcompTestText);
	iEditor->DrawNow();
	}

TKeyResponse CTgb18030AppView::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	if (iEditor)
		return iEditor->OfferKeyEventL (aKeyEvent, aType);
	else
		return EKeyWasNotConsumed;
}

CCoeControl* CTgb18030AppView::ComponentControl(TInt aIndex) const
{
	switch (aIndex)
	{
	case 0:
		return iEditor;

	default:
		return NULL;
	}
}

void CTgb18030AppView::SizeChanged()
{
}

TInt CTgb18030AppView::CountComponentControls() const
{
	return 1; // return number of controls inside this container
}

void CTgb18030AppView::Draw(const TRect& /*aRect*/) const
	{
               // Window graphics context
	CWindowGc& gc = SystemGc();
	           // Area in which we shall draw
	TRect      drawRect = Rect();
			   // Font used for drawing text
	const CFont*     fontUsed;
	
	           // Start with a clear screen
	gc.Clear();
			   // Draw an outline rectangle (the default pen
	           // and brush styles ensure this) slightly
	           // smaller than the drawing area.
	drawRect.Shrink(10,10);		   	
	gc.DrawRect(drawRect);
               // Use the title font supplied by the UI
	fontUsed = iEikonEnv->TitleFont();
	gc.UseFont(fontUsed);
			   // Draw the text in the middle of the rectangle.
	TInt   baselineOffset=(drawRect.Height() - fontUsed->HeightInPixels())/2; 
	gc.DrawText(*iTgb18030Text,drawRect,baselineOffset,CGraphicsContext::ECenter, 0);
               // Finished using the font
	
	gc.DiscardFont();
	}




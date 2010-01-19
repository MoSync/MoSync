/* Copyright (C) 2010 Mobile Sorcery AB

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

#include <MAUI/Layout.h>

#include "Menu.h"
#include "AppStyleMgr.h"
#include "TriggerMenuSelectionAction.h"
#include "CloseMenuAction.h"
#include "MenuItem.h"
#include "IActionSource.h"
#include "SoftKeyBar.h"

using namespace MAUI;

namespace MapDemoUI
{
	const int ItemHeight = 25;
	const int SoftKeysHeight = 25;

	//-------------------------------------------------------------------------
	Menu::Menu( IActionSource* source ) :
	//-------------------------------------------------------------------------
		mCurrent( 0 ),
		mFrame( NULL ),
		mListBox( NULL ),
		mSoftKeys( NULL )
	{
		Vector<Action*> actions = Vector<Action*>( );
		source->enumerateActions( actions );

		mFrame = newobject( Layout, new Layout( 0, 0, mWidth, mHeight, NULL, 1, 2 ) );

		AppStyle* appStyle = AppStyleMgr::getStyle( );
		WidgetSkin* skin = appStyle->getWidgetSkin( );
		Font* font = appStyle->getFont( FontSize_Small, Color::white, false );
		//
		// Listbox
		//
		mListBox = newobject( ListBox, new ListBox( 0, 0, mWidth, mHeight - SoftKeysHeight, NULL, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false ) );
		mListBox->setSkin( skin );

		int count = actions.size( );
		for ( int i = 0; i < count; i++ )
		{
			MenuItem* item = newobject( MenuItem, new MenuItem( actions[i] ) );
			item->setFont( font );
			item->setSkin( skin );
			item->setWidth( mWidth );
			item->setHeight( ItemHeight );
			mListBox->add( item );
		}
		mListBox->setEnabled( true );
		//
		// Soft keys
		//
		mSoftKeys = newobject( SoftKeyBar, new SoftKeyBar( 0, 0, mWidth, SoftKeysHeight, NULL ) );
		mSoftKeys->setLeftAction( newobject( TriggerMenuSelectionAction, new TriggerMenuSelectionAction( this ) ) );
		mSoftKeys->setRightAction( newobject( CloseMenuAction, new CloseMenuAction( this ) ) );
		mSoftKeys->setEnabled( true );
		//
		// Frame
		//
		mFrame->add( mListBox );
		mFrame->add( mSoftKeys );
		mFrame->setEnabled( true );
		mFrame->update( );

		mCurrent = 0;
		updateSelection( );

		setMain( mFrame );
	}

	//-------------------------------------------------------------------------
	Menu::~Menu( )
	//-------------------------------------------------------------------------
	{
		mFrame->getChildren( ).clear( );
		deleteobject( mFrame );
		for ( int i = 0; i < mListBox->getChildren().size( ); i++ )
			deleteobject( mListBox->getChildren( )[i] );
		deleteobject( mListBox );
		deleteobject( mSoftKeys );
	}

	//-------------------------------------------------------------------------
	void Menu::triggerSelectedItem( )
	//-------------------------------------------------------------------------
	{
		if ( mListBox->getChildren( ).size( ) > 0 )
		{
			int selected = mListBox->getSelectedIndex( );
			MenuItem* item = (MenuItem*)mListBox->getChildren( )[selected];
			Action* action = item->getAction( )->clone( );
			close( ); // deletes this
			//
			// Only stack based or cloned objects from here on.
			//
			action->perform( );
			deleteobject( action );
		}
	}

	//-------------------------------------------------------------------------
	bool Menu::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		if ( mSoftKeys->handleKeyPress( keyCode ) )
			return true;

		switch ( keyCode )
		{
		case MAK_UP:
			if ( mCurrent > 0 )
			{
				mCurrent--;
				updateSelection( );
			}
			return true;
		case MAK_DOWN:
			if ( mCurrent < mListBox->getChildren( ).size( ) - 1 )
			{
				mCurrent++;
				updateSelection( );
			}
			return true;
		case MAK_LEFT:
			// close menu
			close( );
			// destructor might have been called, so exit immediately.
			return true;
		case MAK_RIGHT:
			return true;
		case MAK_FIRE:
			triggerSelectedItem( );
			// this might be destroyed here, so we have to just return.
			return true;
		}
		return false;
	}

	//-------------------------------------------------------------------------
	bool Menu::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		if ( mSoftKeys->handleKeyRelease( keyCode ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	void Menu::updateSelection( )
	//-------------------------------------------------------------------------
	{
		int count = mListBox->getChildren( ).size( );
		for ( int i = 0; i < count; i++ )
			mListBox->getChildren( )[i]->setSelected( i == mCurrent );
		if ( count > mCurrent )
			mListBox->setSelectedIndex( mCurrent );
		maUpdateScreen( );
	}
}


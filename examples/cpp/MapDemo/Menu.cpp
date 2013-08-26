/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
		mFrame( NULL ),
		mListBox( NULL ),
		mSoftKeys( NULL ),
		mCurrent( 0 )
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
	bool Menu::handlePointerPress( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( ModalScreen::handlePointerPress( point ) )
			return true;

		if ( mSoftKeys->handlePointerPress( point ) )
		{
			return true;
		}
		else
		{
			Point p;
			p.set(point.x, point.y);
			for(int i = 0; i < mListBox->getChildren().size(); i++)
			{
				if(mListBox->getChildren()[i]->contains(p))
				{
					int index = mListBox->getSelectedIndex();
					if( i == index )
					{
						handleKeyPress( MAK_FIRE );
					}
					else
					{
						mListBox->setSelectedIndex(i);
					}
					break;
				}
			}
		}

		return false;
	}

	//-------------------------------------------------------------------------
	bool Menu::handlePointerMove( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( mSoftKeys->handlePointerMove( point ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	bool Menu::handlePointerRelease( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( mSoftKeys->handlePointerRelease( point ) )
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

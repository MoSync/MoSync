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

namespace UI
{
	const int ItemHeight = 25;
	const int SoftKeysHeight = 25;

	//-------------------------------------------------------------------------
	Menu::Menu( IActionSource* source ) :
	//-------------------------------------------------------------------------
		m_current( 0 ),
		m_frame( NULL ),
		m_listBox( NULL ),
		m_softKeys( NULL )
	{
		Vector<Action*> actions = Vector<Action*>( );
		source->enumerateActions( actions );

		m_frame = newobject( Layout, new Layout( 0, 0, m_width, m_height, NULL, 1, 2 ) );

		AppStyle* appStyle = AppStyleMgr::getStyle( );
		WidgetSkin* skin = appStyle->getWidgetSkin( );
		Font* font = appStyle->getFont( FontSize_Small, Color::white, false );
		//
		// Listbox
		//
		m_listBox = newobject( ListBox, new ListBox( 0, 0, m_width, m_height - SoftKeysHeight, NULL, ListBox::LBO_VERTICAL, ListBox::LBA_NONE, false ) );
		m_listBox->setSkin( skin );

		int count = actions.size( );
		for ( int i = 0; i < count; i++ )
		{
			MenuItem* item = newobject( MenuItem, new MenuItem( actions[i] ) );
			item->setFont( font );
			item->setSkin( skin );
			item->setWidth( m_width );
			item->setHeight( ItemHeight );
			m_listBox->add( item );
		}
		m_listBox->setEnabled( true );
		//
		// Soft keys
		//
		m_softKeys = newobject( SoftKeyBar, new SoftKeyBar( 0, 0, m_width, SoftKeysHeight, NULL ) );
		m_softKeys->setLeftAction( newobject( TriggerMenuSelectionAction, new TriggerMenuSelectionAction( this ) ) );
		m_softKeys->setRightAction( newobject( CloseMenuAction, new CloseMenuAction( this ) ) );
		m_softKeys->setEnabled( true );
		//
		// Frame
		//
		m_frame->add( m_listBox );
		m_frame->add( m_softKeys );
		m_frame->setEnabled( true );
		m_frame->update( );

		m_current = 0;
		updateSelection( );

		setMain( m_frame );
	}

	//-------------------------------------------------------------------------
	Menu::~Menu( )
	//-------------------------------------------------------------------------
	{
		m_frame->getChildren( ).clear( );
		deleteobject( m_frame );
		for ( int i = 0; i < m_listBox->getChildren().size( ); i++ )
			deleteobject( m_listBox->getChildren( )[i] );
		deleteobject( m_listBox );
		deleteobject( m_softKeys );
	}

	//-------------------------------------------------------------------------
	void Menu::triggerSelectedItem( )
	//-------------------------------------------------------------------------
	{
		if ( m_listBox->getChildren( ).size( ) > 0 )
		{
			int selected = m_listBox->getSelectedIndex( );
			MenuItem* item = (MenuItem*)m_listBox->getChildren( )[selected];
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
		if ( m_softKeys->handleKeyPress( keyCode ) )
			return true;

		switch ( keyCode )
		{
		case MAK_UP:
			if ( m_current > 0 )
			{
				m_current--;
				updateSelection( );
			}
			return true;
		case MAK_DOWN:
			if ( m_current < m_listBox->getChildren( ).size( ) - 1 )
			{
				m_current++;
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
		if ( m_softKeys->handleKeyRelease( keyCode ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	void Menu::updateSelection( )
	//-------------------------------------------------------------------------
	{
		int count = m_listBox->getChildren( ).size( );
		for ( int i = 0; i < count; i++ )
			m_listBox->getChildren( )[i]->setSelected( i == m_current );
		if ( count > m_current )
			m_listBox->setSelectedIndex( m_current );
		maUpdateScreen( );
	}
}


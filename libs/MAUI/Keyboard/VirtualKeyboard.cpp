#include <MAUtil/Environment.h>
#include <MAUI/Engine.h>
#include <MAUI/Widget.h>

#include "VirtualKeyboard.h"

namespace MAUI {

VirtualKeyboard::VirtualKeyboard()
	: Widget::Widget( 0, 0, 0, 0, NULL )
{
	MAExtent screenSize = maGetScrSize( );

	/* Set keyboard height to 40% of the screen */
	setHeight( (EXTENT_Y( screenSize ) * 2) / 5 );
	setWidth( EXTENT_X( screenSize ) );
}

VirtualKeyboard::~VirtualKeyboard()
{

}

void
VirtualKeyboard::addLayout(VirtualLayout *layout)
{
	/*  Let height be the same as for the keyboard */
	layout->setHeight( getHeight( ) );
	layout->setWidth( getWidth( ) );

	m_keyLayouts[ layout->getName( ) ] = layout;
}


/* Widget interface */
void
VirtualKeyboard::drawWidget()
{

}

/* PointerListener interface */
void
VirtualKeyboard::pointerPressEvent(MAPoint2d p)
{
	if( !bounds.contains( p.x, p.y ) || !m_curLayout )
	{
		return;
	}

	VirtualKey *pressedKey = m_curLayout->getKey( p.x - bounds.x, p.y - bounds.y );
	if( pressedKey )
	{
		pressedKey->press( );
		m_pressedKeys.add( pressedKey );
		Engine::getSingleton( ).repaint( );
	}
}

void
VirtualKeyboard::pointerMoveEvent(MAPoint2d p)
{

}

void
VirtualKeyboard::pointerReleaseEvent(MAPoint2d p)
{
	VirtualKey *pressedKey = m_curLayout->getKey( p.x - bounds.x, p.y - bounds.y );
	if( pressedKey )
	{
		switch( pressedKey->getType( ) )
		{
			case VirtualKey::KEY:
			case VirtualKey::LINE_FEED:
			{
				const char *outputString = pressedKey->getOutputString( m_curLayout->shift( ) );

				for(int i = 0; outputString[i] != '\0'; i++)
				{
					/* Characters must first be changed then deployed. */
					Environment::getEnvironment( ).fireKeyPressEvent( CHAR_TO_EVENT_MAP[ (unsigned int) outputString[ i ] ], 0 );
				}
			}
			break;
			case VirtualKey::HIDE:
			{
				Engine::getSingleton( ).hideKeyboard( );
			}
			break;
			case VirtualKey::CHANGE_LAYOUT:
			{
				changeLayout( pressedKey->getLayout( ) );
			}
			break;
			case VirtualKey::SHIFT:
			{
				/* Note that these lines must be executed in the correct order */
				CharInput::getCharInput( ).setMode( m_curLayout->shift( ) ? CharInput::LOWERCASE : CharInput::UPPERCASE );
				m_curLayout->toggleShift( );
				Environment::getEnvironment( ).fireKeyPressEvent( MAK_LSHIFT, 0 );
			}
			break;
			case VirtualKey::REMOVE:
			{
				Environment::getEnvironment( ).fireKeyPressEvent( MAK_BACKSPACE, 0 );
			}
			break;
			case VirtualKey::LEFT:
				Environment::getEnvironment( ).fireKeyPressEvent( MAK_LEFT, 0 );
			break;
			case VirtualKey::RIGHT:
				Environment::getEnvironment( ).fireKeyPressEvent( MAK_RIGHT, 0 );
			break;
			case VirtualKey::UP:
				Environment::getEnvironment( ).fireKeyPressEvent( MAK_UP, 0 );
			break;
			case VirtualKey::DOWN:
				Environment::getEnvironment( ).fireKeyPressEvent( MAK_DOWN, 0 );
			break;
			default:
			break;
		}
	}

	/* Remove any pressed keys */
	for(int i = 0; i < m_pressedKeys.size( ); i++)
	{
		m_pressedKeys[i]->unpress( );
		
	}
	m_pressedKeys.clear( );

	/* Repaint the keyboard */
	MAUI::Engine::getSingleton( ).repaint( );
}

void
VirtualKeyboard::runTimerEvent()
{
}

void 
VirtualKeyboard::reset()
{
	changeLayout( MAUtil::String( "DEFAULT" ) );
}

bool
VirtualKeyboard::changeLayout(const MAUtil::String &layoutName)
{
	VirtualLayout *newLayout = m_keyLayouts[ layoutName ];

	if( newLayout != NULL )
	{
		/* If a layout is active, remove it. */
		if( children.size( ) > 0 )
		{
			children.remove( 0 );
		}

		add( newLayout );
		m_curLayout = newLayout;

		/* Repaint keyboard */
		MAUI::Engine &engine = MAUI::Engine::getSingleton( );
		engine.repaint( );
		engine.requestUIUpdate( );

		return true;
	}

	return false;
}

} /* Namespace end */


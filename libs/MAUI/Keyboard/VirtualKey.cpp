#include <MAUtil/String.h>
#include <MAUI/WidgetSkin.h>

#include "VirtualKey.h"

#include <conprint.h>

namespace MAUI {

VirtualKey::VirtualKey(const char *label,
					   const char *outputString,
					   VirtualKey::Type keyType,
					   const char *newLayout,
					   int width,
					   WidgetSkin *icon
					)
	:	m_keyType( keyType ),
		m_width( width )
		
{
	if( newLayout != NULL )
	{
		m_newLayout = MAUtil::String( newLayout );
	}

	strcpy(m_label, label);
	strcpy(m_output, outputString);

	m_icon = icon;

	if(m_keyType == KEY)
	{
		/* Get shifted output */
		shiftChars(m_label, m_shiftedLabel);
		shiftChars(m_output, m_shiftedOutput);
	}

	if(m_keyType == LINE_FEED)
	{
		m_output[0] = '\n';
		m_output[1] = '\0';
	}

	unpress();
}

int
VirtualKey::getWidth() const
{
	return m_width;
}

const char *
VirtualKey::getLabel(bool shifted) const
{
	if( !shifted || m_keyType != KEY )
	{
		return m_label;
	}
	else
	{
		return m_shiftedLabel;
	}
}

WidgetSkin *
VirtualKey::getIcon() const
{
	return m_icon;
}

const char *
VirtualKey::getOutputString(bool shifted) const
{
	if( !shifted || m_keyType != KEY )
	{
		return m_output;
	}
	else
	{
		return m_shiftedOutput;
	}
}


VirtualKey::Type
VirtualKey::getType() const
{
	return m_keyType;
}

MAUtil::String
VirtualKey::getLayout() const
{
	return m_newLayout;
}

void VirtualKey::press()
{
	m_timePressed = maGetMilliSecondCount();
}

void VirtualKey::unpress()
{
	m_timePressed = -1;
}

bool VirtualKey::pressed() const
{
	return m_timePressed >= 0;
}

int VirtualKey::timePressed() const
{
	if( m_timePressed >= 0 )
	{
		return maGetMilliSecondCount() - m_timePressed;
	}
	else
	{
		return -1;
	}
}

void
VirtualKey::shiftChars(const char *chars, char *shiftedChars)
{
	for(unsigned int i = 0; chars[i] != '\0'; i++)
	{
		char c = chars[i];
		if(c >= 'a' && c <= 'z')
		{
			shiftedChars[i] = 'A' + (c - 'a');
		}
		else
		{
			shiftedChars[i] = c;
		}
	}
}

} /* Namespace end */

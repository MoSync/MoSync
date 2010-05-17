#include <mastdlib.h>
#include <mastring.h>

#include "AttributeHandler.h"

namespace MAUI {

AttributeHandler::AttributeHandler(const char **attributes)
{
	m_attributes = attributes;
}

const char *
AttributeHandler::getValueString(const char *name)
{
	int i = 0;
	while(m_attributes[i] != NULL)
	{
		if( strcmp(m_attributes[i], name) == 0 )
		{
			m_status = ATTR_OK;
			return m_attributes[i+1];
		}
		i += 2;
	}

	m_status = ATTR_NOT_FOUND;
	return NULL;
}

int
AttributeHandler::getValueInt(const char *name)
{
	const char *value = getValueString( name );

	if(value != NULL)
	{
		m_status = ATTR_OK;
		return atoi( value );
	}
	else
	{
		m_status = ATTR_NOT_FOUND;
		return 0;
	}
}

float
AttributeHandler::getValueFloat(const char *name)
{
	const char *value = getValueString( name );

	if(value != NULL)
	{
		m_status = ATTR_OK;
		return strtod( value, NULL );
	}
	else
	{
		m_status = ATTR_NOT_FOUND;
		return 0;
	}
}

bool
AttributeHandler::conversionOk() const
{
	return m_status == ATTR_OK;
}

} /* Namespace end */

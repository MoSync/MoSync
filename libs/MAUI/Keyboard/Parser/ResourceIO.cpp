#include <maapi.h>

#include "ResourceIO.h"

namespace MAUI {

ResourceIO::ResourceIO(MAHandle res)
{
	m_size = maGetDataSize( res );
	m_pos = 0;
	m_res = res;
}

unsigned int
ResourceIO::read(char *buffer, unsigned int nbytes)
{
	if( eof( ) )
	{
		return 0;
	}

	unsigned int bytesToRead = nbytes;
	if( m_pos + nbytes >= m_size )
	{
		bytesToRead = m_size - m_pos;

	}

	maReadData( m_res, buffer, m_pos, bytesToRead );

	m_pos += bytesToRead;

	return bytesToRead;
}

unsigned int
ResourceIO::write(const char *buffer, unsigned int nbytes)
{
	return 0;
}

bool
ResourceIO::eof()
{
	return m_pos >= m_size;
}

ResourceIO::~ResourceIO()
{

}

} /* Namespace end */

/*
 * common.cpp
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */

#include <ma.h>
#include "common.h"

void FatalError ( char msg[] )
{
	maPanic( -1, msg );
}

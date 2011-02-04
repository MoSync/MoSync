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

/** 
* \file MapConfig.h
* \brief Configuration header file for MAP library
* \author Lars Åke Vinberg
*/

#ifndef MAPCONFIG_H_
#define MAPCONFIG_H_

//
// Define as 1 to store downloaded tiles in cache in compressed state,
// i.e. the raw PNG or JPG binary data.
// Image resources will then be unpacked at time of rendering.
// This means rendering will be slower but cached tiles will use much
// less memory - possibly 75-90% less.
//
// #define StoreCompressedTilesInCache
//
// Maximum parallel downloaders
//
static const int			MapSourceDownloaders = 5;
//
// Default capacity in MapCache.
//
static const int MapCacheDefaultCapacity = 40;

#endif // MAPCONFIG_H


/* REminiscence - Flashback interpreter
 * Copyright (C) 2005-2006 Gregory Montoir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "locale.h"
#include "resource.h"

Locale::Locale(Version ver)
	: _ver(ver) {
	switch (_ver) {
	case VER_FR:
		_stringsTable = LocaleData::_stringsTableFR;
		_textsTable = LocaleData::_textsTableFR;
		break;
	case VER_EN:
		_stringsTable = LocaleData::_stringsTableEN;
		_textsTable = LocaleData::_textsTableEN;
		break;
	case VER_DE:
		_stringsTable = LocaleData::_stringsTableDE;
		_textsTable = LocaleData::_textsTableDE;
		break;
	case VER_SP:
		_stringsTable = LocaleData::_stringsTableSP;
		_textsTable = LocaleData::_textsTableSP;
		break;
	}
}

const char *Locale::get(int id) const {
	const char *text = 0;
	if (id >= 0 && id < LI_NUM) {
		text = _textsTable[id];
	}
	return text;
}

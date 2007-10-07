/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2007 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

// Usage: include it and then just do a DBGOUT("bla foo bar " << variable << " is awesome.");
// while in debug mode, it will all be print to console. In release mode, DBGOUT does nothing

#ifndef __DEBUG_H__
#	define __DEBUG_H__
#	ifdef _DEBUG
#		include <QDebug>
#		define	DBGOUT(a) qDebug () << QString(__FILE__ ":%1").arg(__LINE__) << "[" << __func__ << "] -" << a
#	else
#		define DBGOUT(a)
#	endif
#endif

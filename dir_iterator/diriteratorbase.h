/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2010 XMMS2 Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef __DIRITERATORBASE_H__
#define __DIRITERATIRBASE_H__

#include <QString>
#include <QPointer>
#include <QIODevice>
#include <QPixmap>

class DirIteratorBase
{
public:
	virtual ~DirIteratorBase () {}

	virtual QString next () = 0;
	virtual QString pathName () = 0;
	virtual bool hasNext () = 0;

	virtual QPointer<QIODevice> entry () = 0;

	// Included because I'm lazy, returns a QPixmap for the current entry.
	// If no pixmap can be createt for the current entry, an empty pixmap is
	// returned
	QPixmap pixmapEntry ();

};

#endif

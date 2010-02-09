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

#include "diriteratorbase.h"
#include "diriterator.h"
#include "archiveiterator.h"
#include <QDataStream>
#include "QFileInfo"


#include <QDebug>

QPixmap
DirIteratorBase::pixmapEntry ()
{
	QPointer<QIODevice> d = entry ();
	if (d == 0) {
		return QPixmap ();
	}

	QByteArray a = d->readAll ();

	QPixmap p;
	p.loadFromData(a);

	if (p.isNull ()) {
		qDebug () << "Size:" << a.size ();
	}

	return p;
}

DirIteratorBase *
DirIteratorBase::open(const QString &path)
{
	QFileInfo info(path);
	if (!info.exists ()) {
		return 0;
	}

	if (info.isDir ()) {
		return new DirIterator (path);
	}

	if (info.isFile ()) {
		ArchiveIterator *iter = new ArchiveIterator (path);
		if (iter && !iter->hasNext ()) {
			delete iter;
			return 0;
		}
		return iter;
	}
}


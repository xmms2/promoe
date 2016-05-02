/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2010-2016 XMMS2 Team
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

#ifndef __ARCHIVEREADDEVICE_H__
#define __ARCHIVEREADDEVICE_H__

#include <QIODevice>

#include "archiveiterator.h"

struct archive;
struct archive_entry;

class ArchiveReadDevice : public QIODevice
{
	Q_OBJECT

public:
	ArchiveReadDevice (ArchiveIterator *parent, const QString &name = QString ());
	~ArchiveReadDevice () {};

	// QIODevice Methods
	qint64 readData(char*, qint64);
	qint64 writeData(const char*, qint64) { return -1; } // read only

	bool atEnd () const { return m_atEnd; }

protected:
	ArchiveIterator *m_iter;

	bool m_atEnd;
};

#endif

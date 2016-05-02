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

#ifndef __ARCHIVEITERATOR_H__
#define __ARCHIVEITERATOR_H__

#include <QIODevice>

#include "diriteratorbase.h"

struct archive;
struct archive_entry;

class ArchiveIterator : public QObject, public DirIteratorBase
{
	Q_OBJECT

public:
	ArchiveIterator (const QString &path);
	ArchiveIterator (QIODevice *);
	~ArchiveIterator ();

	// DirIteratorBase Methods
	QString next ();
	QString pathName ();
	bool hasNext ();
	QPointer<QIODevice> entry ();

	// QIODevice Methods
	qint64 readData(char*, qint64);

protected:
	bool m_valid;
	bool m_atEnd;

	struct archive *m_archive;
	struct archive_entry *m_archive_entry;

	QIODevice *m_dev;
	QPointer<QIODevice> m_current;
	bool finish_init (QIODevice *);
};

#endif

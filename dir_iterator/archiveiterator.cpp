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

#include "archiveiterator.h"
#include "archivereaddevice.h"

#include "promoe_config.h"
#if HAVE_LIBARCHIVE
#include "archive.h"
#include "archive_entry.h"
#endif

#include <QtDebug>

int archive_read_open_qiodevice(struct archive *a, QIODevice *device);

ArchiveIterator::ArchiveIterator (const QString &path) : QObject (),
	DirIteratorBase (), m_current(NULL)
{
	m_dev = new QFile (path);
	m_valid = finish_init (m_dev);
}

ArchiveIterator::ArchiveIterator (QIODevice *d) : QObject (),
	DirIteratorBase (), m_current(NULL), m_dev (NULL)
{
	m_valid = finish_init (d);
}

bool
ArchiveIterator::finish_init (QIODevice *d)
{
	m_archive_entry = NULL;
	m_archive = NULL;
	m_atEnd = false;

#if HAVE_LIBARCHIVE
	m_archive = archive_read_new ();
	if (m_archive == 0) {
		qDebug () << "archive_read_new failed";
		return false;
	}

	if (archive_read_support_compression_all (m_archive) != ARCHIVE_OK) {
		qDebug () << "archive_read_support_compression_all failed";
		return false;
	}
	if (archive_read_support_format_all (m_archive) != ARCHIVE_OK) {
		qDebug () << "archive_read_support_format_all failed";
		return false;
	}
	if (archive_read_open_qiodevice (m_archive, d) != ARCHIVE_OK) {
		qDebug () << "archive_read_open_qiodevice failed";
		return false;
	}

	return true;
#else
	return false;
#endif
}

ArchiveIterator::~ArchiveIterator ()
{
#if HAVE_LIBARCHIVE
	if (m_archive != 0) {
		archive_read_finish (m_archive);
		m_archive = 0;
	}
#endif

	if (m_dev != 0) {
		delete m_dev;
		m_dev = 0;
	}
}

/*
 *  DirIteratorBase methods
 */

QString
ArchiveIterator::next ()
{
#if HAVE_LIBARCHIVE
	if (!m_valid || m_atEnd) {
		return QString ();
	}
	if (m_current != 0) {
		m_current->close ();
		m_current->deleteLater ();
	}
	if (archive_read_next_header (m_archive, &m_archive_entry) != ARCHIVE_OK) {
		m_atEnd = true;
		return QString ();
	}
	QString s(archive_entry_pathname (m_archive_entry));
	m_current = new ArchiveReadDevice (this, s);
	return s;
#else
	return QString ();
#endif
}

QString
ArchiveIterator::pathName ()
{
#if HAVE_LIBARCHIVE
	if (!m_valid || m_atEnd) {
		return QString ();
	}
	QString s(archive_entry_pathname (m_archive_entry));
	return s;
#else
	return QString ();
#endif
}

bool
ArchiveIterator::hasNext ()
{
#if HAVE_LIBARCHIVE
	return !m_atEnd; // workaround, as there is no has_next method in libarchive
#else
	return false;
#endif
}

QPointer<QIODevice>
ArchiveIterator::entry ()
{
#if HAVE_LIBARCHIVE
	return m_current;
#else
	return NULL;
#endif
}

qint64
ArchiveIterator::readData(char *data, qint64 size)
{
#if HAVE_LIBARCHIVE
	if (!m_valid || m_atEnd) {
		qDebug () << "gnarf";
		return -1;
	}
	qint64 ret = archive_read_data (m_archive, data, size);
	return ret;
#else
	return -1;
#endif
}

#include "archiveiterator.moc"

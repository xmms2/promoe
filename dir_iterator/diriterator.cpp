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

#include "diriterator.h"

#include <QFile>

#include <QDebug>

DirIterator::DirIterator (const QDir & dir) : DirIteratorBase ()
{
	m_dir = dir;
	finish_init ();
}

DirIterator::DirIterator (const QString & path) : DirIteratorBase ()
{
	m_dir = QDir (path);
	finish_init ();
}

DirIterator::~DirIterator ()
{
	if (m_device != 0)
		delete m_device;
}

void
DirIterator::finish_init ()
{
	if (!m_dir.exists ()) {
		m_exists = false;
		return;
	}

	m_exists = true;
	m_current = -1;
	entryList = m_dir.entryList ();
	m_device = 0;
}

QString
DirIterator::next ()
{
	if (!m_exists || (m_current +1) >= entryList.size ())
		return QString();

	m_current++;

	if (m_device != 0) {
		m_device->close ();
		delete m_device;
		m_device = 0;
	}

	return entryList.at (m_current);
}

bool
DirIterator::hasNext ()
{
	return (m_exists && ((m_current +1) < entryList.size ()));
}

QString
DirIterator::pathName ()
{
	if (!m_exists || m_current >= entryList.size ())
		return QString ();

	return entryList.at (m_current);
}

QPointer<QIODevice>
DirIterator::entry ()
{
	if (!m_exists || m_current >= entryList.size ())
		return 0;

	if (m_device != 0) {
		if (m_device->isOpen ()) {
			return m_device;
		} else {
			return 0;
		}
	}

	m_device = new QFile (m_dir.absoluteFilePath (entryList.at (m_current)));
	if (!m_device->open (QIODevice::ReadOnly)) {
		qDebug() << entryList.at (m_current);
		return 0;
	}

	return m_device;
}

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


#include "archivereaddevice.h"

#include "promoe_config.h"

#include <QtDebug>

ArchiveReadDevice::ArchiveReadDevice (ArchiveIterator *parent,
                                      const QString &name)
	: QIODevice (parent), m_iter(parent), m_atEnd(false)
{
	setObjectName (name);
	setOpenMode (QIODevice::ReadOnly);
}

qint64
ArchiveReadDevice::readData (char* buf, qint64 size)
{
	if (!isOpen ()) {
		return -1;
	}
	qint64 ret = m_iter->readData (buf, size);
	if (ret <= 0) {
		m_atEnd = true;
	}
	return ret;
}

#include "archivereaddevice.moc"

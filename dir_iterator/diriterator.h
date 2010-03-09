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

#ifndef __DIRITERATOR_H__
#define __DIRITERATOR_H__

#include "diriteratorbase.h"

#include <QObject>
#include <QDir>
#include <QStringList>


class DirIterator : public DirIteratorBase
{
public:
	DirIterator (const QDir & dir);
	DirIterator (const QString & path);
	virtual ~DirIterator ();

	virtual QString next ();
	virtual QString pathName ();
	virtual bool hasNext ();

	virtual QPointer<QIODevice> entry ();

private:
	void finish_init ();

	bool m_exists;
	int m_current;
	QDir m_dir;
	QStringList entryList;
	QIODevice * m_device;

};

#endif

/**
 *  This file is a part of Promoe, an XMMS2 client
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
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

#include "xclientcache.h"

#include "entryinfo.h"

#include <QHash>
#include <QString>
#include <QVariant>

EntryInfo::EntryInfo (QWidget *parent, XClientCache *cache, uint32_t id)
                      : QDialog (parent)
{
	setupUi (this);
	setAttribute (Qt::WA_DeleteOnClose);

	m_cache = cache;
	m_id = id;

	connect (cache, SIGNAL (entryChanged (uint32_t)),
	         this, SLOT (mlib_entry_changed (uint32_t)));

	connect (cache, SIGNAL (entryRemoved (uint32_t)),
	         this, SLOT (mlib_entry_removed (uint32_t)));

	getInfo ();
}

void
EntryInfo::getInfo ()
{
	QHash<QString, QVariant> tmp = m_cache->get_info (m_id);

	//FIXME "url" value ist wrong. tight be an error in clientcache
	filenameEdit->setText (tmp.value ("url").toString ());
	titleEdit->setText (tmp.value ("title").toString ());
	artistEdit->setText (tmp.value ("artist").toString ());
	albumEdit->setText (tmp.value ("album").toString ());
	// commentEdit->setText ();
	yearEdit->setText (tmp.value ("date").toString ());
	trackEdit->setText (tmp.value ("tracknr").toString ());
	genreEdit->setText (tmp.value ("genre").toString ());
}

void
EntryInfo::setId (uint32_t id)
{
	m_id = id;
	getInfo ();
}

void
EntryInfo::mlib_entry_changed (uint32_t id)
{
	if (id == m_id) {
		// the metadata for the current entry changed,
		// so we update the dialog to display the current values
		// TODO: think of a good way to handle changed values as soon as
		// we allow editing of metadata
		getInfo ();
	}
}

void
EntryInfo::mlib_entry_removed (uint32_t id)
{
	// if the mlib no longer contains our item, we have to remove it
	if (id == m_id) {
		close ();
	}
}

#include "entryinfo.moc"

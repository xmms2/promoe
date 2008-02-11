/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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
#include "xclient.h"

#include <QObject>
#include <QIcon>
#include <QPixmap>
#include <QHash>
#include <QList>
#include <QVariant>
#include <QPixmapCache>
#include <QSettings>

XClientCache::XClientCache (XClient *client) : QObject (client)
{
	QSettings s;
	connect (client, SIGNAL (gotConnection (XClient *)),
	         this, SLOT (got_connection (XClient *)));
	QPixmapCache::setCacheLimit (s.value ("core/pixmapcache").toInt ());
}

void
XClientCache::got_connection (XClient *client)
{
	m_client = client;
	client->playback ()->signalPlaytime () (
	         Xmms::bind (&XClientCache::handle_playtime, this));
	client->playback ()->getPlaytime () (
	         Xmms::bind (&XClientCache::handle_playtime, this));

	client->medialib ()->broadcastEntryChanged () (
	         Xmms::bind (&XClientCache::handle_mlib_entry_changed, this));
}

bool
XClientCache::handle_medialib_info_error (const std::string &error, uint32_t id)
{
    /* we probably couldn't find that entry, let's remove it */
    m_info.remove (id);
    emit entryRemoved (id);
    return true;
}

bool
XClientCache::handle_medialib_info (const Xmms::PropDict &info)
{
	int32_t id = info.get<int32_t> ("id");
	QHash<QString, QVariant> hash = XClient::convert_propdict (info);

	m_info.insert (id, hash);
	emit entryChanged (id);

	return true;
}

void
XClientCache::extra_info_set (uint32_t id, const QString &name,
							   const QVariant &value)
{
	m_extra_info[id][name] = value;
}

QVariant
XClientCache::extra_info_get (uint32_t id, const QString &name)
{
	return m_extra_info[id][name];
}

void
XClientCache::invalidate (uint32_t id)
{
    m_info.remove (id);
}

void
XClientCache::invalidate_all ()
{
    m_info.clear ();
}

bool
XClientCache::handle_bindata (const Xmms::bin &data, const QString &id)
{
	QPixmap i;
	i.loadFromData (data.c_str (), data.size());

	if (i.isNull ()) {
		return true;
	}

	/* conserve memory client side */
	if (i.width () < 300) {
		i = i.scaledToWidth (300, Qt::SmoothTransformation);
	}

	QPixmapCache::insert (id, i);

	QList<uint32_t> ids = m_icon_map[id];
	for (int i = 0; i < ids.size (); i++) {
		emit entryChanged (ids.at (i));
	}

	return true;
}

QIcon
XClientCache::get_icon (uint32_t id)
{
	return QIcon (get_pixmap (id));
}

QPixmap
XClientCache::get_pixmap (uint32_t id)
{
	if (m_info[id].contains ("picture_front")) {
		QString hash = m_info[id]["picture_front"].toString ();
		QPixmap p;

		if (!QPixmapCache::find (hash, p)) {
			m_client->bindata ()->retrieve (hash.toStdString ()) (
			     boost::bind (&XClientCache::handle_bindata, this, _1, hash));
			QPixmapCache::insert (hash, QPixmap ());
			m_icon_map[hash].append (id);
		}

		return p;
	}
	return QPixmap ();
}

QHash<QString, QVariant>
XClientCache::get_info (uint32_t id)
{
	if (!m_info.contains (id)) {
		m_client->medialib ()->getInfo (id) (
		               Xmms::bind (&XClientCache::handle_medialib_info, this),
		               boost::bind (&XClientCache::handle_medialib_info_error,
					                this, _1, id));
		m_info[id] = QHash<QString, QVariant> ();
	}

	return m_info[id];
}

bool
XClientCache::handle_mlib_entry_changed (const uint32_t &id)
{
	m_client->medialib ()->getInfo (id) (Xmms::bind (&XClientCache::handle_medialib_info, this));
	return true;
}


bool
XClientCache::handle_playtime (const unsigned int &tme)
{
	emit playtime(tme);
	return true;
}

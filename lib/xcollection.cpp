/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <xmmsclient/xmmsclient++.h>
#include "compat.h"

#include "xcollection.h"
#include "xcollection_p.h"
#include "xclient.h"

#include <QHash>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <QVariant>

/*
 * Private implemention to hide <xmmsclient/xmmsclient++> from collection.h
 */

XCollection::Private::Private (XCollection *collection, XClient* client) 
            : QObject (collection)
{
	m_client = client;
}

bool
XCollection::Private::on_collection_modified (const Xmms::Dict &value)
{
	QString newname = QString ();
	QHash<QString, QVariant> tmp = XClient::convert_dict (value);
	int type = tmp["type"].toInt ();
	QString name = tmp["name"].toString ();

	// FIXME: handle other namespaces than "Playlists"
	if (tmp["namespace"].toString () != "Playlists") {
		return true;
	}

	if (type == XMMS_COLLECTION_CHANGED_RENAME)
		newname = tmp["newname"].toString ();

	// FIXME: use namespace parameter
	switch (type) {
		case XMMS_COLLECTION_CHANGED_ADD:
			m_playlists.append (name);
			break;
		case XMMS_COLLECTION_CHANGED_REMOVE:
			m_playlists.removeAll (name);
			break;
		case XMMS_COLLECTION_CHANGED_RENAME:
			m_playlists.removeAll (name);
			m_playlists.append (newname);
			break;
		case XMMS_COLLECTION_CHANGED_UPDATE:
			// do nothing here
			break;
		default:
			qDebug ("Unhandled collection change type %i", type);
	}

	m_playlists.sort ();

/*
	qDebug ("---");
	foreach (QString s, m_playlists) {
		qDebug (s.toAscii ());
	}
*/

	emit collectionModified (tmp["name"].toString (),
	                         tmp["namespace"].toString (),
							 type, newname);

	return true;
}

bool
XCollection::Private::handle_idlist_created (const Xmms::Coll::Coll &idlist)
{
	// we add a idlist, we don't want to sort it in any way
	// I hope this does the trick.
	std::list < std::string > tmp;

	m_client->playlist ()->addCollection (idlist, tmp);

	return true;
}


/*
 * The XCollection class
 */
XCollection::XCollection (XClient * client) : QObject ( client)
{
	m_client = client;
	d = new XCollection::Private (this, client);

	connect (client, SIGNAL (gotConnection (XClient *)),
	         this, SLOT (on_connect (XClient *)));

	// emit signals from private implementation as our own
	connect (d, SIGNAL (collectionModified(QString, QString, int, QString)),
	         this, SIGNAL (collectionModified(QString, QString, int, QString)));
	connect (d, SIGNAL (activePlaylistChanged (QString, QString)),
	         this, SIGNAL (activePlaylistChanged (QString, QString)));


	if (client->isConnected ()) {
		on_connect (client);
	}
}

void
XCollection::on_connect (XClient *client)
{
	client->collection ()->broadcastCollectionChanged ()
	        (Xmms::bind (&XCollection::Private::on_collection_modified, d));

	client->collection ()->list ("Playlists")
	        (Xmms::bind (&XCollection::Private::handle_playlists_list, d));

	client->playlist ()->currentActive ()
	        (Xmms::bind (&XCollection::Private::handle_active_pls_changed, d));
	client->playlist ()->broadcastLoaded ()
	        (Xmms::bind (&XCollection::Private::handle_active_pls_changed, d));

	m_client = client;
}

bool
XCollection::remove (QString name, QString ns) {
	if (!m_client->isConnected ()) return false;

	m_client->collection ()->remove (name.toStdString (), ns.toAscii ());

	return true;
}

/*
 *  idList (Playlist) stuff
 */
QString
XCollection::activePlaylist ()
{
	return d->m_activePlaylist;
}


bool
XCollection::Private::handle_playlists_list (const Xmms::List< std::string > &list)
{
	m_playlists.clear ();

#if HAVE_XMMSV
	for (Xmms::List< std::string >::const_iterator iter = list.begin();
	     iter != list.end(); iter ++) {
		m_playlists.append (XClient::stdToQ(*iter));
	}
#else
	for (list.first (); list.isValid (); ++list) {
		m_playlists.append (XClient::stdToQ(*list));
	}
#endif
	m_playlists.sort ();

	return true;
}


QStringList
XCollection::list (QString ns) {
	// FIXME: use the ns parameter.
	// We will need to handle querying the serverside playlists bettter...
	return d->m_playlists;
}


// FIXME: Probably should be in another file
bool
XCollection::setActivePlaylist (QString name) {
	if (!m_client->isConnected ()) return false;

	m_client->playlist ()->load (XClient::qToStd (name));

	return true;
}

bool
XCollection::Private::handle_active_pls_changed (const std::string &name) {
	QString tmp = m_activePlaylist;
	m_activePlaylist = XClient::stdToQ (name);

	emit activePlaylistChanged (m_activePlaylist, tmp);

	return true;
}

// FIXME: should be done in a more generic way
bool
XCollection::addIdlist (QString name) {
	if (!m_client->isConnected ()) return false;

	m_client->playlist ()->create (name.toStdString ());

	return true;
}

bool
XCollection::playlistAddUrl (QString url, QString plsname)
{
	if (plsname == "") {
		plsname = d->m_activePlaylist;
	}

	m_client->playlist ()->addUrl (url.toStdString (),
	                               plsname.toStdString ());

	return true;
}

bool
XCollection::playlistAddUrl (QUrl url, QString plsname)
{
	//TODO: more tests if file is valid
	if (!url.isValid ()) {
		return false;
	}

	if (plsname == "") {
		plsname = d->m_activePlaylist;
	}

	m_client->playlist ()->addUrl (url.toString ().toStdString (),
	                               plsname.toStdString ());

	return true;
}

bool
XCollection::playlistClear (QString name)
{
	if (name == "") {
		name = d->m_activePlaylist;
	}
	m_client->playlist ()->clear (XClient::qToStd (name));

	return true;
}

bool
XCollection::addPlsFile (QUrl url)
{
	if (url.scheme ().isEmpty ()) {
		//the protocol identifier is missing
		//we guess we might be local, so just add file://
		url.setScheme ("file");
	}
	m_client->collection ()->idlistFromPlaylistFile
	          (std::string (url.toEncoded ()))
	          (Xmms::bind (&XCollection::Private::handle_idlist_created, d));

	return true;
}

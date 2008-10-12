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

#include "xmmsqt4.h"
#include "XMMSHandler.h"
#include "playlistmodel.h"

#include <cstdlib>
#include <string>
#include <iostream>

#include <QErrorMessage>
#include <QHash>

XMMSHandler &XMMSHandler::getInstance ()
{
	static XMMSHandler singleton(NULL, "Promoe_Main");
	return singleton;
}

XMMSHandler::XMMSHandler (QObject *parent, const std::string &name) : XClient (parent, name)
{
	connect_handler();
}

bool
XMMSHandler::connect_handler (const char *ipcpath, const bool &sync, QWidget *parent)
{
/*	try {
		m_client.connect(path);
		//TODO reenable checking of path
		//m_client.connect (path ? path : "");
	}
	catch (Xmms::connection_error& e) {
		QErrorMessage *err = new QErrorMessage ();
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		delete err;
		return false;
	}

	m_client.setMainloop (new XmmsQT4 (m_client.getConnection ()));
*/
	//TODO must be moved elsewere later
	m_playlist_model = new PlaylistModel(NULL, this, "_active");

	connect(ipcpath, sync, parent);

	using Xmms::bind;
//	m_client->playback.broadcastVolumeChanged () (
//	                    bind (&XMMSHandler::volume_changed, this));

	return true;
}

void
XMMSHandler::playlistAddURL (const QString &s)
{
	m_client->playlist.addUrl (s.toAscii ().constData ()) ();
}

void
XMMSHandler::DictToQHash (const std::string &key,
                          const Xmms::Dict::Variant &value,
                          QHash<QString, QString> &hash)
{
	if (value.type () == typeid (int32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< int32_t > (value)));
	} else if (value.type () == typeid (uint32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< uint32_t > (value)));
	} else if (value.type () == typeid (std::string)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::fromUtf8 (boost::get< std::string > (value).c_str ()));
	}
}

/*
void
XMMSHandler::PropDictToQHash (const std::string &key,
                              const Xmms::Dict::Variant &value,
                              const std::string &source,
                              QHash<QString, QString> &hash)
{
	if (value.type () == typeid (int32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< int32_t > (value)));
	} else if (value.type () == typeid (uint32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< uint32_t > (value)));
	} else {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::fromUtf8 (boost::get< std::string > (value).c_str()));
	}
}
*/
/*
uint
XMMSHandler::medialibQuery (QString q)
{
	XMMSResultDictList *r = m_xmmsc->medialib_select (q.toUtf8 ());
	r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_select));
	m_mlibqs->append (r);
}

void
XMMSHandler::medialib_select (XMMSResultDictList *res)
{
	QList<QHash<QString, QString> > l;

	for (;res->listValid (); res->listNext()) {
		QHash<QString, QString> h(DictToQHash (static_cast<XMMSResultDict *>(res)));
		l.append (h);
	}

	emit medialibResponse (res->getCID (), l);
}
*/


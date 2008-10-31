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
	//TODO must be moved elsewere later
	m_playlist_model = new PlaylistModel(NULL, this, "_active");

	connect(ipcpath, sync, parent);

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


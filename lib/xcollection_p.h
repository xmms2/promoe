/**
 *  This file is a part of Promoe
 *
 *  Copyright (C) 2008 Thomas Frauendorfer
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

// these headers are placed in here, so that the moc files are created and
// that these Objects can emit signals

#ifndef __XCOLLECTION_P_H__
#define __XCOLLECTION_P_H__

#include <xmmsclient/xmmsclient++.h>

#include "xcollection.h"

#include <QObject>
#include <QStringList>
#include <QString>

class XCollection::Private : public QObject {
    Q_OBJECT

	public:
		Private (XCollection *collection);

		bool on_collection_modified (const Xmms::Dict &value);
		bool handle_playlists_list (const Xmms::List< std::string > &list);
		bool handle_active_pls_changed (const std::string &name);

		QStringList m_playlists;
		QString m_activePlaylist;

	signals:
    	void collectionModified (QString collection, QString ns, int type,
    	                         QString newname);

		void activePlaylistChanged (QString newActive, QString oldActive);
};

#endif

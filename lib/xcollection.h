/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2008 Thomas Frauendorfer
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

#ifndef __XCollection_H__
#define __XCollection_H__

#include "xclient.h"

#include <QObject>
class QString;
class QStringList;

class XCollection : public QObject
{
	Q_OBJECT

	public:
		XCollection (XClient *client);
		QStringList list (QString ns = "Playlists");

		bool setActivePlaylist (QString name);
		bool addIdlist (QString name);
		bool remove (QString name, QString ns);

	signals:
		void collectionModified (QString collection, QString ns, int type,
		                         QString newname);

	protected slots:
		void on_connect (XClient *);

	private:
		bool on_collection_modified (const Xmms::Dict &value);
		bool handle_playlists_list (const Xmms::List< std::string > &list);
		XClient *m_client;
		QStringList m_playlists;
};

#endif

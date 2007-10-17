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


#include <xmmsclient/xmmsclient++.h>
#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>
#include <QIcon>
#include <QMimeData>
#include <QSettings>
#include <QUrl>
#include <QFileInfo>

#include "playlistmodel.h"

PlaylistModel::PlaylistModel (QObject *parent, XClient *client, const QString &name) : QAbstractItemModel (parent)
{
//	m_columns.append ("#");
	m_columns.append ("Artist");
	m_columns.append ("Title");

//	m_colfallback.append ("");
	m_colfallback.append ("");
	m_colfallback.append ("url");

	m_cached_size.append (QSize ());
	m_cached_size.append (QSize ());
	m_cached_size.append (QSize ());

	connect (client, SIGNAL(gotConnection (XClient *)), this, SLOT (got_connection (XClient *))); 
	connect (client->cache (), SIGNAL(entryChanged (uint32_t)), this, SLOT (entry_changed (uint32_t)));
	
	if (name == QLatin1String ("_active")) {
        m_isactive = true;
	}
	
    m_name = name;
    
    if (client->isConnected ()) {
        got_connection (client);
    }
}

void
PlaylistModel::set_playlist (const QString &name)
{
    if (name == QLatin1String ("_active")) {
        m_isactive = true;
        m_client->playlist ()->currentActive () (Xmms::bind (&PlaylistModel::handle_current_pls, this));
    } else {
        m_isactive = false;
    }
	m_name = name;
	m_client->playlist ()->listEntries (XClient::qToStd (name)) (Xmms::bind (&PlaylistModel::handle_list, this));
}

bool
PlaylistModel::handle_current_pls (const std::string &name)
{
    if (m_name == QLatin1String ("_active")) {
        m_name = XClient::stdToQ (name);
    }
    
    return true;
}

void
PlaylistModel::got_connection (XClient *client)
{
    if (m_isactive) {
        client->playlist ()->currentActive () (Xmms::bind (&PlaylistModel::handle_current_pls, this));
    }
    
	client->playlist ()->listEntries (XClient::qToStd (m_name)) (Xmms::bind (&PlaylistModel::handle_list, this));
	client->playlist ()->currentPos () (Xmms::bind (&PlaylistModel::handle_update_pos, this));

	client->playlist ()->broadcastChanged () (Xmms::bind (&PlaylistModel::handle_change, this));
	client->playlist ()->broadcastCurrentPos () (Xmms::bind (&PlaylistModel::handle_update_pos, this));

	client->playlist ()->broadcastLoaded () (Xmms::bind (&PlaylistModel::handle_pls_loaded, this));

	m_client = client;
}

bool
PlaylistModel::handle_pls_loaded (const std::string &name)
{
	if (m_isactive) {
		m_client->playlist ()->listEntries (name)
									   (Xmms::bind (&PlaylistModel::handle_list, this));
        m_name = XClient::stdToQ (name);
	}

	return true;
}

bool
PlaylistModel::handle_update_pos (const uint32_t &pos)
{
	m_current_pos = pos;
	emit dataChanged(index (pos, 0), index (pos, m_columns.size ()));
	return true;
}

QList<QString>
PlaylistModel::columns () const
{
	return m_columns;
}

void
PlaylistModel::setColumns (const QList<QString> &new_columns)
{
	m_columns = new_columns;
	reset ();
}

void
PlaylistModel::setColumnFallback (const QList<QString> &new_columns)
{
	m_colfallback = new_columns;
	reset ();
}

bool
PlaylistModel::handle_change (const Xmms::Dict &chg)
{
	int32_t change = chg.get<int32_t> ("type");
	int32_t pos = 0, npos = 0;
	uint32_t id = 0;
	QString s;

	if (chg.contains ("position")) {
		pos = chg.get<int32_t> ("position");
	}

	if (chg.contains ("id")) {
		id = chg.get<uint32_t> ("id");
	}

	if (chg.contains ("name")) {
		s = XClient::stdToQ (chg.get<std::string> ("name"));
	}
    
	if (s != m_name) {
		return true;
	}

	QModelIndex idx = QModelIndex ();

	switch (change) {
		case XMMS_PLAYLIST_CHANGED_ADD:
			beginInsertRows (idx, pos, pos);
			m_plist.append (id);
			endInsertRows ();
			break;
		case XMMS_PLAYLIST_CHANGED_INSERT:
			beginInsertRows (idx, pos, pos);
			m_plist.insert (pos, id);
			endInsertRows ();
			break;
		case XMMS_PLAYLIST_CHANGED_MOVE:
			npos = chg.get<int32_t> ("newposition");

			beginRemoveRows (idx, pos, pos);
			m_plist.removeAt (pos);
			endRemoveRows ();

			beginInsertRows (idx, npos, npos);
			m_plist.insert (npos, id);
			endInsertRows ();

			if (pos < npos && pos)
				pos --;

			emit entryMoved (index (pos, 0), index (npos, 0));

			break;
		case XMMS_PLAYLIST_CHANGED_REMOVE:
            m_client->cache ()->invalidate (m_plist[pos]);
			beginRemoveRows (idx, pos, pos);
			m_plist.removeAt (pos);
			endRemoveRows ();			
			break;
		case XMMS_PLAYLIST_CHANGED_SHUFFLE:
		case XMMS_PLAYLIST_CHANGED_SORT:
		case XMMS_PLAYLIST_CHANGED_CLEAR:
            m_client->cache ()->invalidate_all ();
			m_client->playlist ()->listEntries () (Xmms::bind (&PlaylistModel::handle_list, this));
			break;
	}

	return true;
}

bool
PlaylistModel::handle_list (const Xmms::List< unsigned int > &list)
{
	beginRemoveRows (QModelIndex (), 0, m_plist.size ());
	m_plist.clear ();
	endRemoveRows ();

	int i = 0;
	for (list.first (); list.isValid (); ++list) {
		i ++;
	}
	beginInsertRows (QModelIndex (), 0, i);
	for (list.first (); list.isValid (); ++list) {
		m_plist.append (*list);
	}

	endInsertRows ();

	return true;
}

QModelIndexList
PlaylistModel::get_idxlist_by_id (uint32_t id)
{
	QModelIndexList ret;

	QList<uint32_t> l = getPosById (id);
	for (int i = 0; i < l.count (); i++) {
		ret.append (index (l.at (i), 0));
	}
	return ret;
}

QList<uint32_t>
PlaylistModel::getPosById (uint32_t id)
{
	QList<uint32_t> ret;
	int32_t pos = m_plist.indexOf (id);

	while (pos != -1) {
		ret.append (pos);
		pos = m_plist.indexOf (id, pos + 1);
	}

	return ret;
}

void
PlaylistModel::entry_changed (uint32_t id)
{
	QList<uint32_t> pos = getPosById (id);
	for (int i = 0; i < pos.size (); i ++) {
		QModelIndex idx1 = index (pos.at (i), 0);
		QModelIndex idx2 = index (pos.at (i), m_columns.size ());
		emit dataChanged(idx1, idx2);
	}
}

int
PlaylistModel::columnCount (const QModelIndex &parent) const
{
	return m_columns.size ();
}

int
PlaylistModel::rowCount (const QModelIndex &parent) const
{
	if (!parent.isValid ()) {
		return m_plist.size ();
	}

	return 0;
}

QModelIndex
PlaylistModel::parent (const QModelIndex &idx) const
{
	return QModelIndex ();
}

QModelIndex
PlaylistModel::index (int row, int column, const QModelIndex &parent) const 
{
	if (!parent.isValid ()) {
		if (row > (m_plist.size () - 1))
			return QModelIndex ();
		if (row < 0)
			return QModelIndex ();
		return createIndex (row, column, -1);
	}

	return QModelIndex ();
}

QVariant
PlaylistModel::data (const QModelIndex &index, int role) const
{
	if (!index.isValid ()) {
		return QVariant ();
	}

	if (index.row () >= m_plist.size ()) {
		return QVariant ();
	}

	if (role == MedialibIdRole) {
		return QVariant (m_plist[index.row ()]);
	}

	if (role == Qt::SizeHintRole) {
		if (m_cached_size[index.column ()].isValid ()) {
			return QVariant (m_cached_size[index.column ()]);
		}
		return QVariant ();
	}

	if (role == Qt::DisplayRole || role == Qt::ToolTipRole) {
		QString key = m_columns[index.column ()].toLower ();
		QString fallkey = m_colfallback[index.column ()].toLower ();

		if (key == "#") {
			return QVariant (index.row ());
		} else {
			unsigned int id = m_plist[index.row ()];
			PlaylistModel *fake = const_cast<PlaylistModel*> (this);
			QHash<QString, QVariant> d = fake->m_client->cache ()->get_info (id);

			if (d.contains (key)) {
				return QVariant (d[key]);
			} else if (d.contains (fallkey)) {
				return QVariant (d[fallkey]);
			}

			return QVariant ();
		}
	} else if (role == CurrentEntryRole) {
		int i = m_current_pos;

		if (index.row () == i)
			return QVariant (true);
		return QVariant (false);
	}

	return QVariant ();
}

QStringList
PlaylistModel::mimeTypes () const
{
	QStringList l ("application/x-xmms2poslist");
	l << "application/x-xmms2mlibid";
	l << "text/uri-list";
	return l;
}

QMimeData *
PlaylistModel::mimeData (const QModelIndexList &list) const
{
	QMimeData *ret = new QMimeData ();
	QByteArray ba;
	QDataStream stream (&ba, QIODevice::WriteOnly);

	QList<int> l;
	for (int i = 0; i < list.size (); i ++) {
		QModelIndex idx = list.at (i);
		if (idx.column () != 0)
			continue;
		l.append (idx.row ());
	}

	stream << l;
	ret->setData ("application/x-xmms2poslist", ba);

	return ret;
}

bool
PlaylistModel::dropMimeData (const QMimeData *data,
							 Qt::DropAction action,
							 int row, int column,
							 const QModelIndex & parent)
{
	if (parent.internalId () != -1 && parent.isValid ()) {
		return false;
	}

	if (data->hasFormat ("application/x-xmms2poslist")) {
		if (!parent.isValid ())
			return false;

		QByteArray ba = data->data ("application/x-xmms2poslist");
		QDataStream stream (&ba, QIODevice::ReadOnly);
		QList<int> l;
		stream >> l;
		qSort (l);
		int target = parent.row ();

		int mod = 0;

		while (l.size ()) {
			int orow = l.takeAt (0) - mod;
			m_client->playlist ()->moveEntry (orow, target) ();
			if (orow < target) {
				mod ++;
			} else {
				target ++;
			}
		}
		return true;
	} else if (data->hasFormat ("application/x-xmms2mlibid")) {
		QByteArray ba = data->data ("application/x-xmms2mlibid");
		QDataStream stream (&ba, QIODevice::ReadOnly);
		QList<int> l;
		stream >> l;

		int target;

		if (parent.isValid ())
			target = parent.row () + 1;
		else
			target = m_plist.size () + 1;

		while (l.size ()) {
			int id = l.takeAt (0);
			if (target >= m_plist.size ()) {
				m_client->playlist ()->addId (id) ();
			} else {
				m_client->playlist ()->insertId (target ++, id) ();
			}
		}
		return true;
	} else if (data->hasFormat ("text/uri-list")) {
		int target;

		if (parent.isValid ())
			target = parent.row () + 1;
		else
			target = m_plist.size () + 1;

		QList<QUrl> l = data->urls ();

		qSort (l);

		for (int i = 0; i < l.size (); i++) {
			QFileInfo fi (l.at (i).toLocalFile ());
			std::string s ("file:///");
			s.append (fi.absoluteFilePath ().toLocal8Bit ());
			if (fi.isFile ()) {
				if (target >= m_plist.size ()) {
					m_client->playlist ()->addUrl (s) ();
				} else {
					m_client->playlist ()->insertUrl (target ++, s) ();
				}
			} else if (fi.isDir ()) {
				m_client->playlist ()->addRecursive (s) ();
			}
		}

		return true;
	}
	return false;
}

Qt::DropActions
PlaylistModel::supportedDropActions () const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QVariant
PlaylistModel::headerData (int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) {
		return QVariant ();
	}

	if (orientation == Qt::Horizontal) {
		if (section <= m_columns.size ())
			return QVariant (m_columns[section]);
	}

	return QVariant ();
}

Qt::ItemFlags
PlaylistModel::flags (const QModelIndex &idx) const
{
	unsigned int id = m_plist[idx.row ()];
	PlaylistModel *fake = const_cast<PlaylistModel*> (this);
	QHash<QString, QVariant> d = fake->m_client->cache ()->get_info (id);

	Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;

	if (idx.isValid ()) {
		f |= Qt::ItemIsDropEnabled;
	}
	if (d.contains ("status") && d["status"] != XMMS_MEDIALIB_ENTRY_STATUS_NOT_AVAILABLE) {
		f |= Qt::ItemIsEnabled;
	}

	return f;
}

QList<uint32_t>
PlaylistModel::get_all_id ()
{
	return m_plist;
}

void
PlaylistModel::removeRows (QModelIndexList index_list)
{
	QList<uint32_t> idlist;

	for (int i = 0; i < index_list.size (); ++i) {
		QModelIndex idx = index_list.at(i);
		if (idx.column () != 0)
			continue;

		idlist.append (idx.row ());
	}
	qSort (idlist);

	/* Update of m_plist is done in handle_change through server notification */
	for (int i = idlist.size () - 1; i >= 0; --i){
		m_client->playlist ()->removeEntry (idlist.at(i));
	}
}

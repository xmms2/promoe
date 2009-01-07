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


#ifndef __PLAYLIST_MODEL_H__
#define __PLAYLIST_MODEL_H__

#include <xmmsclient/xmmsclient++.h>

class XClient;

#include <QAbstractTableModel>
#include <QVariant>
#include <QHash>
#include <QIcon>

/**
 * @class PlaylistModel playlistmodel.h
 * @brief A model that represents a playlist on the server
 *
 * This model will show the playlist and update it according to the
 * changes from the server. This can be subclassed to be customized.
**/
class PlaylistModel : public QAbstractItemModel
{
	Q_OBJECT
	public:
	    /**
	     * The constructor for the PlaylistModel.
	     * @param parent The parent QObject for this model
	     * @param client The XClient object to be used in order to get the updates
	     * @param n The name of the playlist that this model should show
	    **/
		PlaylistModel (QObject *parent, XClient *client, const QString &n = "_active");

		enum {
			DisplayRole = Qt::DisplayRole,
			ToolTipRole = Qt::ToolTipRole,
			CurrentEntryRole = 200,
			MedialibIdRole,
			AvailableRole
		};
		
		int rowCount (const QModelIndex &parent) const;
		int columnCount (const QModelIndex &parent) const;
		QVariant data (const QModelIndex &index, int role = DisplayRole) const;
		QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex &) const;
		QModelIndex parent (const QModelIndex &) const;
		QModelIndex index (int row, int column, const QModelIndex &idx = QModelIndex ()) const;
		QMimeData *mimeData (const QModelIndexList &list) const;
		bool dropMimeData (const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
		Qt::DropActions supportedDropActions () const;
		QStringList mimeTypes () const;

		QModelIndex currentPos () {return index (m_current_pos, 0);}

		/**
		 * Set the columns that should be shown in the view.
		 * @param columns A list of property keys. i.e. "artist", "album"
		**/
		void setColumns (const QList <QString> &columns);
		
		/**
		 * Set fallback columns. A fallback column is what should be shown
		 * if the first column is not available for that entry.
		 * @param columns A list of property keys.
		**/
		void setColumnFallback (const QList <QString> &columns);

        /**
         * Return the current columns.
        **/
		QList<QString> columns () const;
		
		/**
		 * Return a list of all entry ids that are currently in the list.
		**/
		QList<uint32_t> get_all_id ();

		void set_cached_size (int i, const QSize &size) {
			m_cached_size[i] = size;
		};

		QSize cached_size (int i) const {
			return m_cached_size[i];
		};
        
        /**
         * Return a list of QModelIndex for the entry. Since one
         * entry can be in the Playlist multiple times we need to
         * return a list of indexes.
         * @ 
        **/
		QModelIndexList get_idxlist_by_id (uint32_t);

		QModelIndex current_playlist_pos () const {
			return index (m_current_pos, 0);
		};

		void set_playlist (const QString &);

		void removeRows (QModelIndexList);

		uint32_t getPlaytimeForSelection(const QModelIndexList &index_list);

	protected:
		XClient *m_client;
		QList < unsigned int > m_plist;
		QList < uint32_t > getPosById (uint32_t id);
		QList < QString > m_columns;
		QList < QString > m_colfallback;
		
	signals:
		void entryMoved (const QModelIndex &, const QModelIndex &);
		void currentPosChanged (QModelIndex);
		void totalPlaytime (uint32_t seconds, bool isExact);

	public slots:
		void got_connection (XClient *);
		void entry_changed (uint32_t);

	private:
		bool handle_list (const Xmms::List< unsigned int > &list);
		bool handle_change (const Xmms::Dict &chg);
#if (XMMS_IPC_PROTOCOL_VERSION > 10)
		bool handle_update_pos (const Xmms::Dict &pos);
#else
		bool handle_update_pos (const unsigned int &pos);
#endif
		bool handle_pls_loaded (const std::string &);
        bool handle_current_pls (const std::string &);

		void getInfo (unsigned int id) const;

		void emitTotalPlaytime ();

		uint32_t m_current_pos;
        bool m_isactive;

		QList<QSize> m_cached_size;

		QString m_name;

};

#endif

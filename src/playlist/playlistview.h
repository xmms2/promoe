/**
 *  This file is a part of Prome, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
 *  Copyright (C) 2007,2008 Thomas Frauendorfer
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

#ifndef __PLAYLISTVIEW_H__
#define __PLAYLISTVIEW_H__

//include "Playlist.h"
#include <xmmsclient/xmmsclient++.h>
#include "Skin.h"
#include <QObject>
#include <QAbstractItemDelegate>
#include <QListView>
#include <QWidget>

class PlaylistDelegate : public QAbstractItemDelegate {
	Q_OBJECT

	public:
		PlaylistDelegate (QObject *parent = NULL);
		~PlaylistDelegate () { }

		void paint( QPainter * painter, const QStyleOptionViewItem & option,
		            const QModelIndex & index ) const;

		QSize sizeHint ( const QStyleOptionViewItem &option,
		                 const QModelIndex &index ) const;
};


class PlaylistView : public QListView {
	Q_OBJECT

	public:
		PlaylistView (QWidget *parent);
		~PlaylistView () {}

		void setModel (QAbstractItemModel *model);

	public slots:
		void contextMenuEvent (QContextMenuEvent *e);

		void handleStatus (const Xmms::Playback::Status st);
		void invertSelection (void);
		void cropSelected (void);
		void removeSelected (void);
		void settingsSaved (void);
		void setPixmaps (Skin *skin);

	protected:
		void mouseDoubleClickEvent (QMouseEvent *event);

	private:
		Xmms::Playback::Status m_status;
		QFont *m_font;
		QFontMetrics *m_fontmetrics;
		QColor m_color_active;
		QColor m_color_selected;
		QColor m_color_normal;
		QColor m_color_normal_bg;
};


#endif

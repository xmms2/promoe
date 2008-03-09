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

#ifndef __PLAYLISTMENU_H__
#define __PLAYLISTMENU_H__

class PlaylistMenu;

//#include "XMMSHandler.h"
#include "PixWidget.h"
#include "Skin.h"

#include <QWidget>

class PlaylistMenuBar : public PixWidget {
	Q_OBJECT
	public:
		PlaylistMenuBar (QWidget *parent, uint id) : PixWidget (parent)
		{
			m_id = id;
		}
	public slots:
		void setPixmaps (Skin *skin) { m_pixmap = skin->getPls (m_id); }

	private:
		uint m_id;
};

class PlaylistMenuButton : public QWidget {
	Q_OBJECT
	public:
		PlaylistMenuButton (PlaylistMenu *, uint pix1, uint pix2);
		~PlaylistMenuButton ();

		void paintEvent (QPaintEvent *event);
		void setPixmaps (Skin *skin);

		void setOver (void) { m_pixmap = m_pixmap2; }
		void setNoOver (void) { m_pixmap = m_pixmap1; }
		void clicked (void) { emit activated (); }

	signals:
		void activated (void);

	private:
		uint m_pixid1;
		uint m_pixid2;

		PlaylistMenu *m_menu;

		QPixmap m_pixmap1;
		QPixmap m_pixmap2;
		QPixmap m_pixmap;
};

class PlaylistMenu : public PixWidget {
	Q_OBJECT
	public:
		PlaylistMenu (QWidget *, uint, uint);
		~PlaylistMenu () {}

		void addButton (PlaylistMenuButton *b);
		void rmButton (PlaylistMenuButton *b);

		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);
		void mouseMoveEvent (QMouseEvent *);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		uint m_pixid;
		uint m_dec;
		QPixmap m_dpixmap;
		QList<PlaylistMenuButton *> m_items;
		QWidget *m_expanded;

		PlaylistMenuBar *m_decbar;

};

#endif

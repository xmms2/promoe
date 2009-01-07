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

#ifndef __PLAYLISTCONTROLS_H__
#define __PLAYLISTCONTROLS_H__

#include <QWidget>
#include <QMap>

class SmallTimeDisplay;
class QPaintEvent;
class QPixmap;

typedef QMap<int, QPixmap> PixmapMap;

class PlaylistControls : public QWidget {
	Q_OBJECT

	public:
		PlaylistControls (QWidget *parent);

		void setNumbers (const PixmapMap &p);
		void setPixmapFont (const PixmapMap &p);

	public slots:
		void setSelectionPlaytime (uint32_t);
		void setPlaylistPlaytime (uint32_t, bool isExact);

	signals:
		// emitted when buttons are clicked
		void prev ();
		void play ();
		void pause ();
		void stop ();
		void next ();
		void eject ();

		void toggleTime ();
		//connected to internal timedisplay
		void setDisplayTime (int);

	private slots:
		void paintEvent (QPaintEvent *event);

	private:
		uint32_t m_playlist_playtime;
		bool is_playlist_playtime_exact;
		uint32_t m_selection_playtime;
		PixmapMap m_font;

		SmallTimeDisplay *m_timedisplay;
};

#endif

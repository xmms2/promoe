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
class QPixmap;

typedef QMap<int, QPixmap> PixmapMap;

class PlaylistControls : public QWidget {
	Q_OBJECT

	public:
		PlaylistControls (QWidget *parent);

		void setNumbers (const PixmapMap &p);

	public slots:
		void setSelectedLength (int);
		void setPlaylistLength (int);

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

	private:
		int m_playlist_length;
		int m_selected_length;

		SmallTimeDisplay *m_timedisplay;
};

#endif

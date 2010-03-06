/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2010 XMMS2 Team
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

#ifndef __PLAYLISTWINDOW_H__
#define __PLAYLISTWINDOW_H__

#include "basewindow.h"

// Qt classes
class QEvent;
class QResizeEvent;
class QPoint;

// our own classes
class MainWindow;
class PlaylistWidget;
class PlaylistShade;

class PlaylistWindow : public BaseWindow {
	Q_OBJECT

	public:
		PlaylistWindow (QWidget *parent);
		~PlaylistWindow () {}

		void setActive (bool);

	signals:
		// setTime is used to set playtime in playlistcontrols
		void setDisplayTime (int seconds);

	public slots:
		void switchDisplay (void);

	protected:
		void activeWindowInEvent (QEvent *event);
		void activeWindowOutEvent (QEvent *event);
		void resizeEvent (QResizeEvent *event);

	private:
		PlaylistWidget *m_playlist;
		PlaylistShade *m_shaded;
};

#endif // __PLAYLISTWINDOW_H__

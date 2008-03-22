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

#ifndef __PLAYLISTWINDOW_H__
#define __PLAYLISTWINDOW_H__

// Qt classes
#include "basewindow.h"

class QEvent;
class QMoveEvent;
class QResizeEvent;
class QHideEvent;
class QShowEvent;
class QPoint;

// our own classes
class Button;
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
		void visibilityChanged(bool visible);

	public slots:
		void switchDisplay (void);

	protected:
		void hideEvent (QHideEvent *event);
		void showEvent (QShowEvent *event);
		void enterEvent (QEvent *event);
		void leaveEvent (QEvent *event);
		void moveEvent (QMoveEvent *event);
		void resizeEvent (QResizeEvent *event);

	private:
		PlaylistWidget *m_playlist;
		PlaylistShade *m_shaded;

		Button *m_shadebtn;
		Button *m_closebtn;
};

#endif // __PLAYLISTWINDOW_H__

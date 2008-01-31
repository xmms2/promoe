/**
 *  This file is a part of Prome, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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

#ifndef _PLAYLISTWINDOW_
#define _PLAYLISTWINDOW_

// Qt classes
 #include <QMainWindow>

class QEvent;
class QMouseEvent;
class QMoveEvent;
class QResizeEvent;

// our own classes
class Button;
class MainWindow;
class PlaylistWidget;
class PlaylistShade;

class PlaylistWindow : public QMainWindow {
	Q_OBJECT

	public:
		PlaylistWindow (QWidget *parent);
		~PlaylistWindow () {}

		void setActive (bool);

		void mousePressEvent (QMouseEvent *event);
		void mouseMoveEvent (QMouseEvent *event);
		void enterEvent (QEvent *event);
		void leaveEvent (QEvent *event);
		void moveEvent (QMoveEvent *event);
		void resizeEvent (QResizeEvent *event);

	public slots:
		void switchDisplay (void);
		void togglePL (void);

	private:
		PlaylistWidget *m_playlist;
		PlaylistShade *m_shaded;

		int m_diffx;
		int m_diffy;

		Button *m_shadebtn;
		Button *m_closebtn;
		uint getOffset (void);
		MainWindow *m_mw;

};

#endif // _PLAYLISTWINDOW_

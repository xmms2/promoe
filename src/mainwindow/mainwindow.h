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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include "basewindow.h"

#include <QSettings>
class QWidget;
class QMouseEvent;

class MainDisplay;
class ShadedDisplay;
class PlaylistWindow;
class EqualizerWindow;

class MainWindow : public BaseWindow
{
	Q_OBJECT
	public:
		MainWindow (QWidget *parent);
		~MainWindow (void);

		MainDisplay *getMD () { return m_display; }
		ShadedDisplay *getSD () { return m_shaded; }
		PlaylistWindow *getPL () { return m_playlistwin; }
		EqualizerWindow *getEQ () { return m_equalizer; }


		void raisePL (void);
		void moveEvent (QMoveEvent *event);

		void attachWidgets ();

	public slots:
		void switchDisplay ();
		void mouseMoveEvent (QMouseEvent *event);

	private:
		bool isShaded (void) { QSettings s; return s.value("MainWindow/shaded").toBool(); }
		void setShaded (bool b) { QSettings s; return s.setValue("MainWindow/shaded", b); }
		MainDisplay *m_display;
		ShadedDisplay *m_shaded;
		EqualizerWindow *m_equalizer;
		PlaylistWindow *m_playlistwin;

		QMap<BaseWindow *,QPoint> m_attachedWidgets;
};


#endif

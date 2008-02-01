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

#ifndef __EQUALIZERWINDOW_H__
#define __EQUALIZERWINDOW_H__

#include <QMainWindow>

class QWidget;
class QMouseEvent;
class QMoveEvent;

class MainWindow;
class EqualizerWidget;

class EqualizerWindow : public QMainWindow
{
	Q_OBJECT
	public:
		EqualizerWindow(QWidget *parent);
		~EqualizerWindow();
		void mouseMoveEvent(QMouseEvent *);
		void mousePressEvent(QMouseEvent *);
		void moveEvent(QMoveEvent *event);

	public slots:
		void setEnabled (void);

	private:
		MainWindow *m_mw;
		EqualizerWidget *m_equalizer;
		int m_diffx;
		int m_diffy;
};

#endif // __EQUALIZERWINDOW_H__

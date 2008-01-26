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

#ifndef __EQUALIZER_H__
#define __EQUALIZER_H__

class Skin;
class Button;
class MainWindow;
class TitleBar;
class ToggleButton;
class Slider;

#include <iostream>

#include <QPixmap>
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMainWindow>

#include "Display.h"

class EqualizerWidget : public QWidget
{
	Q_OBJECT
	public:
		EqualizerWidget(QWidget *parent);
		~EqualizerWidget();
		void paintEvent (QPaintEvent *event);

	public slots:
		void setPixmaps(Skin *skin);
	private:
		QPixmap m_pixmap;
		QPixmap m_graph;
		ToggleButton *m_enable;
		ToggleButton *m_auto;
		Button *m_preset;
		Slider *m_preamp;
		Slider *m_bands[10];
};

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


#endif

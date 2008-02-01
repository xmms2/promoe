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

#ifndef __EQUALIZERWIDGET_H__
#define __EQUALIZERWIDGET_H__

#include <QWidget>
class QPixmap;
class QPaintEvent;

class Skin;
class Button;
class ToggleButton;
class Slider;

//#include <iostream>

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


#endif

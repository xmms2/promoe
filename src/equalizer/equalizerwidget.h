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
class QString;
class QVariant;
class QPixmap;
class QPaintEvent;

#include "VolumeSlider.h"
class XConfig;
class Skin;
class Button;
class ToggleButton;

//#include <iostream>

class EqualizerSlider : public Slider
{
	Q_OBJECT

	public:
	EqualizerSlider (QWidget*, uint, uint, uint, uint, int, int, int);

	signals:
	void numberedValueChanged (int value, int id);

	protected slots:
	void on_self_value_changed (int value);

	private:
	int m_id;
};

class EqualizerWidget : public QWidget
{
	Q_OBJECT

	public:
		EqualizerWidget(QWidget *parent);
		~EqualizerWidget();
		void paintEvent (QPaintEvent *event);

	public slots:
		void setPixmaps(Skin *skin);

	protected slots:
		void serverConfigChanged (QString key, QString value);
		void setEqualizerEnabled (bool enabled);
		void updateServerPreamp (int value);
		void updateServerBands (int value, int id);

	private:
		XConfig *m_xconfig;
		QPixmap m_pixmap;
		QPixmap m_graph;
		ToggleButton *m_enable;
		ToggleButton *m_auto;
		Button *m_preset;
		Slider *m_preamp;
		EqualizerSlider *m_bands[10];
};


#endif

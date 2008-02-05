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

#ifndef __VOLUMESLIDER_H__
#define __VOLUMESLIDER_H__

class Slider;
class MainDisplay;
class Button;

#include "Button.h"
#include "PixWidget.h"
#include "Skin.h"

class SliderButton : public Button
{
	Q_OBJECT
	public:
		SliderButton (QWidget *parent, uint normal, uint pressed, bool dir);
		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);
		void mouseReleaseEvent (QMouseEvent *);
		void setPos (uint);


	public slots:
		//void setValue (uint volume_base100);

	private:
		void changePixmap (bool pressed);
		Slider *m_slider;
		bool m_vertical;
		int m_diff;
};

class Slider : public PixWidget
{
	Q_OBJECT
	public:
		Slider (QWidget *parent, uint min, uint max, uint on, uint off, int, int);

		void paintEvent (QPaintEvent *event);

		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);
		void mouseReleaseEvent (QMouseEvent *);

	public slots:
		void setPixmaps (Skin *skin);
		void setPos (int value, bool tell);
		void setValue (int value);

	signals:
		void valueChanged (int val);

	private:
		void updatePos (QMouseEvent *event);
		void changePixmap ();

		QPixmap m_pixmap_slider;
		SliderButton *m_button;

		Skin *m_skin;

		uint m_value_index;
		int m_value;

		int m_position;

		uint m_name_min;
		uint m_name_max;
		uint m_name_on;
		uint m_name_off;

		int m_min;
		int m_max;

		bool m_vertical;
};

#endif

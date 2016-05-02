/**
 *  This file is a part of Promoe, an XMMS2 CLient
 *
 *  Copyright (C) 2008-2016 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef __PIXMAPSLIDER_H__
#define __PIXMAPSLIDER_H__

#include <QAbstractSlider>
#include <QList>
#include <QPoint>

#include "buttonpixmaps.h"

class QWidget;
class QPixmap;
class QPaintEvent;
class QMouseEvent;

typedef QList<QPixmap> QPixmapList;

class PixmapSlider : public QAbstractSlider
{
	Q_OBJECT
	public:
		PixmapSlider (QWidget *parent);
		~PixmapSlider () {}

		void setValue (int val);

		void setBackground (const QPixmap &);
		void setBackground (const QPixmapList &);
		QPixmapList backgrounds () const { return m_backgrounds; }

		void setButton (const ButtonPixmaps &);

		void setSliders (QPixmap normal, QPixmap pressed = QPixmap ());
		QPixmap normalSlider () const
			{ return m_button.pixmap (ButtonPixmaps::Normal); }
		QPixmap pressedSlider () const
			{ return m_button.pixmap (ButtonPixmaps::Pressed); }

		void setSliderOffset (QPoint offset) { m_slider_offset = offset; }

	protected slots:
		void paintEvent (QPaintEvent *event);

		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);
		void mouseReleaseEvent (QMouseEvent *);

	protected:
		int sliderMovePosition (QMouseEvent *);

		int sliderPositionFromValue ();
		int sliderValueFromPosition (int pos);
		int backgroundIndex ();

	private:
		QPixmapList m_backgrounds;
		ButtonPixmaps m_button;

		QPoint m_slider_offset;
		int m_background_index;
};

#endif

/**
 *  This file is a part of Promoe, an XMMS2 Client
 *
 *  Copyright (C) 2008 Thomas Frauendorfer
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

#include "pixmapslider.h"

#include <QPixmap>
#include <QStyle>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

PixmapSlider::PixmapSlider (QWidget *parent) : QAbstractSlider (parent)
{
	setSliderDown (false);
}

void
PixmapSlider::setBackground (const QPixmap &pixmap)
{
	QPixmapList list;
	list.append (pixmap);
	setBackground (list);
}

void
PixmapSlider::setBackground (const QPixmapList &list)
{
	m_backgrounds = list;
	m_background_index = -1;
}

void
PixmapSlider::setSliders (QPixmap normal, QPixmap pressed)
{
	m_normal = normal;
	m_pressed = pressed;
}

void
PixmapSlider::mouseMoveEvent (QMouseEvent *event)
{
	if (!isSliderDown ()) {
		event->ignore ();
		return;
	}

	int val = sliderMovePosition (event);

	if (val == value ()) {
		return;
	}
	QAbstractSlider::setValue (val);
}

void
PixmapSlider::mousePressEvent (QMouseEvent *event)
{
	if (event->button () != Qt::LeftButton) {
		event->ignore ();
		return;
	}

	setSliderDown (true);

	int val = sliderMovePosition (event);

	if (val == value ()) {
		return;
	}
	QAbstractSlider::setValue (val);

	update ();
}

void
PixmapSlider::mouseReleaseEvent (QMouseEvent *event)
{
	if (event->button () != Qt::LeftButton) {
		event->ignore ();
		return;
	}

	int val = sliderMovePosition (event);

	QAbstractSlider::setValue (val);

	setSliderDown (false);

	update ();
}

int
PixmapSlider::sliderMovePosition (QMouseEvent *event)
{
	if (orientation () == Qt::Vertical) {
		return sliderValueFromPosition (event->y() - m_pressed.height () /2);
	} else {
		return sliderValueFromPosition (event->x() - m_pressed.width () /2);
	}

}

void
PixmapSlider::setValue (int val)
{
	if (isSliderDown ()){
		return;
	}
	QAbstractSlider::setValue (val);
}

void
PixmapSlider::paintEvent (QPaintEvent *event)
{
	QPainter p;
	p.begin (this);
	// draw background
	if (!m_backgrounds.isEmpty ()) {
		int bg_idx = backgroundIndex ();
		QPixmap bg = m_backgrounds[bg_idx];
		// only redraw the complete background, if the index changed
		if (bg_idx == m_background_index) {
			p.drawPixmap (event->rect (), bg, bg.rect ());
		} else {
			m_background_index = bg_idx;
			p.drawPixmap (rect (), bg, bg.rect ());
		}
	}
	// draw slider
	QPixmap *slider = isSliderDown () ? &m_pressed : &m_normal;
	QRect rect (slider->rect ());
	if (orientation () == Qt::Vertical) {
		rect.moveTop (sliderPositionFromValue ());
		rect.moveLeft (1);
	} else {
		rect.moveLeft (sliderPositionFromValue ());
		rect.moveTop (1);
	}
	p.drawPixmap (rect , *slider, slider->rect ());
	p.end ();
}

int
PixmapSlider::sliderPositionFromValue ()
{
	int span;
	if (orientation () == Qt::Vertical) {
		span = height () - m_normal.height ();
	} else {
		span = width () - m_normal.width ();
	}

    return QStyle::sliderPositionFromValue (minimum (), maximum (),
	                                        sliderPosition (), span,
											invertedAppearance ());
}

int
PixmapSlider::sliderValueFromPosition (int pos)
{
	int span;
	if (orientation () == Qt::Vertical) {
		span = height () - m_normal.height ();
	} else {
		span = width () - m_normal.width ();
	}

	return  QStyle::sliderValueFromPosition (minimum (), maximum (), pos,
	                                         span, invertedAppearance ());
}

int
PixmapSlider::backgroundIndex ()
{
	if (m_backgrounds.isEmpty ()) {
		return -1;
	}

	return QStyle::sliderPositionFromValue (minimum (), maximum (),
	                                        sliderPosition (),
	                                        m_backgrounds.size () - 1, false);
}

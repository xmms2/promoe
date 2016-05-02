/**
 *  This file is a part of Promoe, an XMMS2 Client
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

#include "pixmapslider.h"

#include <QPixmap>
#include <QStyle>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

PixmapSlider::PixmapSlider (QWidget *parent) : QAbstractSlider (parent)
{
	setSliderDown (false);
	m_slider_offset = QPoint ();
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
PixmapSlider::setButton (const ButtonPixmaps &p)
{
	m_button = p;
}

void
PixmapSlider::setSliders (QPixmap normal, QPixmap pressed)
{
	m_button.addPixmap (normal, ButtonPixmaps::Normal);
	m_button.addPixmap (pressed, ButtonPixmaps::Pressed);
}

void
PixmapSlider::mouseMoveEvent (QMouseEvent *event)
{
	if (!isSliderDown ()) {
		event->ignore ();
		return;
	}

	int val = sliderMovePosition (event);
	if (val != value ()) {
		QAbstractSlider::setValue (val);
	}

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
	if (val != value ()) {
		QAbstractSlider::setValue (val);
	}


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
	if (val != value ()) {
		QAbstractSlider::setValue (val);
	}


	setSliderDown (false);

	update ();
}

int
PixmapSlider::sliderMovePosition (QMouseEvent *event)
{
	int ret;
	QPixmap p = m_button.pixmap (ButtonPixmaps::Pressed);
	if (orientation () == Qt::Vertical) {
		ret = sliderValueFromPosition (event->y() - p.height () /2);
	} else {
		ret = sliderValueFromPosition (event->x() - p.width () /2);
	}

	// Make sliders snap to center. Usefull for equalizer sliders and balance
	const int centerValue = 0;
	if ((ret > centerValue - pageStep ()) &&
	    (ret < centerValue + pageStep ())) {
		ret = centerValue;
	}

	return ret;
}

void
PixmapSlider::setValue (int val)
{
	if (isSliderDown ()){
		return;
	}
	if (val == value()) {
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
		m_background_index = bg_idx;
		p.drawPixmap (0, 0, bg.width(), bg.height(), bg);
	}
	// draw slider
	QPixmap slider = m_button.pixmap (isSliderDown () ? ButtonPixmaps::Pressed
	                                                  : ButtonPixmaps::Normal);
	QRect rect (slider.rect ());
	if (orientation () == Qt::Vertical) {
		rect.moveTop (sliderPositionFromValue () + m_slider_offset.y ());
		rect.moveLeft (m_slider_offset.x ());
	} else {
		rect.moveLeft (sliderPositionFromValue () + m_slider_offset.x ());
		rect.moveTop (m_slider_offset.y ());
	}
	p.drawPixmap (rect , slider, slider.rect ());
	p.end ();
}

int
PixmapSlider::sliderPositionFromValue ()
{
	int span;
	QPixmap p = m_button.pixmap (ButtonPixmaps::Normal);
	if (orientation () == Qt::Vertical) {
		span = height () - p.height ();
	} else {
		span = width () - p.width ();
	}

    return QStyle::sliderPositionFromValue (minimum (), maximum (),
	                                        sliderPosition (), span,
											invertedAppearance ());
}

int
PixmapSlider::sliderValueFromPosition (int pos)
{
	int span;
	QPixmap p = m_button.pixmap (ButtonPixmaps::Normal);
	if (orientation () == Qt::Vertical) {
		span = height () - p.height ();
	} else {
		span = width () - p.width ();
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

#include "pixmapslider.moc"

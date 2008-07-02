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

#include "timedisplay.h"

#include <QPainter>
#include <QPixmap>
#include <QMap>


TimeDisplay::TimeDisplay (QWidget *parent) : AbstractTimeDisplay (parent)
{
	setFixedSize (63, 13);

	m_d1_x_pos = 12;
	m_d2_x_pos = 24;
	m_d3_x_pos = 42;
	m_d4_x_pos = 54;
}

SmallTimeDisplay::SmallTimeDisplay (QWidget *parent)
                 : AbstractTimeDisplay (parent)
{
	setFixedSize (28, 6);

	m_d1_x_pos = 5;
	m_d2_x_pos = 10;
	m_d3_x_pos = 18;
	m_d4_x_pos = 23;
}

/*
 * This method takes the playtime in seconds
 */
void
AbstractTimeDisplay::setTime (int time)
{
	// Hack to make display hours and seconds instead of seconds and minutes
	// if time (or reversetime) is 100 Minutes or longer
	if ((time >= 6000) || (time <= -6000)) {
		time /= 60;
	}

	if (m_time == time) return;

	m_time = time;
	update ();
}

void
AbstractTimeDisplay::setPixmaps (const PixmapMap &p) {
	if (p.size () < 11) {
		// This shouldn't happen, if it does then there is a bug in Skin.cpp
		qDebug ("TimeDisplay: PixmapMap has not enough elements");
		return;
	}
	m_pixmaps = p;
}

void
AbstractTimeDisplay::mouseReleaseEvent (QMouseEvent *event)
{
	emit clicked();
}

void
AbstractTimeDisplay::paintEvent (QPaintEvent *event)
{
	QPainter paint;
	paint.begin (this);

	if (m_time < 0) {
		// draw minus
		paint.drawPixmap (0, 0, m_pixmaps[11]);
	} else {
		// draw blank
		paint.drawPixmap (0, 0, m_pixmaps[10]);
	}
	uint showtime = abs(m_time);
	if (showtime < 6000) {
		// draw minutes
		uint min = showtime / 60;
		paint.drawPixmap (m_d1_x_pos, 0, m_pixmaps[min/10]);
		paint.drawPixmap (m_d2_x_pos, 0, m_pixmaps[min%10]);
		// draw seconds
		uint sec = showtime % 60;
		paint.drawPixmap (m_d3_x_pos, 0, m_pixmaps[sec/10]);
		paint.drawPixmap (m_d4_x_pos, 0, m_pixmaps[sec%10]);
	} else {
		// Just give up and draw '-' if min-variable would become 100 or bigger
		paint.drawPixmap (m_d1_x_pos, 0, m_pixmaps[11]);
		paint.drawPixmap (m_d2_x_pos, 0, m_pixmaps[11]);
		paint.drawPixmap (m_d3_x_pos, 0, m_pixmaps[11]);
		paint.drawPixmap (m_d4_x_pos, 0, m_pixmaps[11]);

	}
	paint.end ();
}

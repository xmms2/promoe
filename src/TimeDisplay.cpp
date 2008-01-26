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

#include "MainDisplay.h"
#include "TimeDisplay.h"
#include "NumberDisplay.h"

#include "MainWindow.h"

#include <QPen>

TimeDisplay::TimeDisplay (QWidget *parent, int time) : PixWidget (parent)
{
	uint w = 12;//+78;
	
	m_w = w;
	
	m_pixmap = QPixmap (m_w, 13);

	setMinimumSize (m_w, 13);
	setMaximumSize (m_w, 13);
	move (37, 26);

	//FIXME: let this be the parent
/* XXX: colon disappear, how make transparent?
	m_number_min = new NumberDisplay (this, 24, 0);
	m_number_min->move (10, 0);
	m_number_sec = new NumberDisplay (this, 24, 0);
	m_number_sec->move (78-37, 0);
*/

	m_number_min = new NumberDisplay (parent, this,24, 0);
	m_number_min->move (37+10, 26);
	m_number_sec = new NumberDisplay (parent, this,24, 0);
	m_number_sec->move (78, 26);

	connect (m_number_min, SIGNAL(clicked()), parent, SLOT(toggleTime()));
	connect (m_number_sec, SIGNAL(clicked()), parent, SLOT(toggleTime()));
}
void TimeDisplay::setPixmaps (Skin *skin)
{
	drawMinus();
	m_number_min->setPixmaps (skin);
	m_number_sec->setPixmaps (skin);
}
TimeDisplay::~TimeDisplay () 
{
}
void TimeDisplay::setTime (int time)
{
	if (m_time == time) return;
	m_time = time;

	uint showtime = abs(time);
	uint sec, min;

	sec = (showtime / 1000) % 60;
	min = (showtime / 1000) / 60;

	m_number_min->setNumber (min / 10, min % 10);
	m_number_sec->setNumber (sec / 10, sec % 10);

	drawMinus ();
}
void
TimeDisplay::drawMinus ()
{
	Skin *skin = Skin::getInstance ();

	// Draw background
	QBrush b (skin->getNumber (10));

	QPainter paint;
	paint.begin (&m_pixmap);
	paint.fillRect (m_pixmap.rect (), b);
	MainDisplay *md = dynamic_cast<MainDisplay *>(parent());

	if (md->getMW()->isTimemodeReverse()) {// draw a minus sign
		if (skin->getNumberSize() < 12) { // Skin hasn't got any, draw a line in correct color.
			QByteArray a = skin->getPLeditValue("normal");
			QColor c;
			c.setNamedColor(a);
			QPen pen(c);
			paint.setPen(pen);
			paint.drawLine (3,6,8,6);
		} else {
			paint.drawPixmap (0, 0, skin->getNumber (11));
		}
	}
	paint.end();

	update ();
}
void
TimeDisplay::mousePressEvent (QMouseEvent *event)
{
}
void
TimeDisplay::mouseReleaseEvent (QMouseEvent *event)
{
	emit clicked();
	drawMinus ();
}


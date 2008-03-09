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

#include "NumberDisplay.h"

#include "TimeDisplay.h"
#include "Skin.h"

#include <QPainter>

NumberDisplay::NumberDisplay (QWidget *parent, TimeDisplay *td,uint w, uint startpx) : PixWidget (parent)
{
	m_td = td;
	m_w = w;
	m_startpx = startpx;

	m_pixmap = QPixmap (m_w, 13);
	
	setMinimumSize (m_w, 13);
	setMaximumSize (m_w, 13);

	setNumber (0,0);
}

void
NumberDisplay::setPixmaps (Skin *skin)
{
	QBrush b (skin->getNumber (10));

	QPainter paint;
	paint.begin (&m_pixmap);
	paint.fillRect (m_pixmap.rect (), b);
	paint.drawPixmap (m_startpx, 0, skin->getNumber (m_n1));
	paint.drawPixmap (m_startpx+12, 0, skin->getNumber (m_n2));
	paint.end();

	update ();
}

void
NumberDisplay::setNumber (uint n1, uint n2)
{
	if (m_n1 != n1 || m_n2 != n2) {
		Skin *skin = Skin::getInstance ();

		m_n1 = n1;
		m_n2 = n2;

		setPixmaps (skin);
	}
}

NumberDisplay::~NumberDisplay ()
{
}
void
NumberDisplay::mousePressEvent (QMouseEvent *event)
{
}
void
NumberDisplay::mouseReleaseEvent (QMouseEvent *event)
{
//	TimeDisplay *td = dynamic_cast<TimeDisplay *>(parent());
	emit clicked();
	m_td->drawMinus ();
}


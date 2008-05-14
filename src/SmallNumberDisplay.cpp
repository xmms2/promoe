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

#include "SmallNumberDisplay.h"
#include "Skin.h"

#include <QPainter>

SmallNumberDisplay::SmallNumberDisplay (QWidget *parent, int w) : PixWidget (parent)
{
	m_w = w;
	setMinimumSize (w, 6);
	setMaximumSize (w, 6);
	m_pixmap = QPixmap (w, 6);
}

void
SmallNumberDisplay::setPixmaps (Skin *skin)
{
	drawNumber ();
}

void
SmallNumberDisplay::setNumber (int num, int len)
{
	snprintf (m_nums, len+1, "%02d", num);
	m_num = len;

	drawNumber ();

	update ();
}

void
SmallNumberDisplay::drawNumber ()
{
	Skin *skin = Skin::getInstance ();

	QPainter paint;
	paint.begin (&m_pixmap);
	paint.drawPixmap (m_pixmap.rect (),
					  skin->getItem (Skin::TEXTBG),
					  m_pixmap.rect ());

	for (int i = 0; i < m_num; i++) {
		paint.drawPixmap (QRect (i*5, 0, 4, 6),
						  skin->getLetter (m_nums[i]),
						  skin->getLetter (m_nums[i]).rect ());
	}

	paint.end ();
}


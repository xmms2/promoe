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

#include "clutterbar.h"
#include "Skin.h"

ClutterBar::ClutterBar (QWidget *parent) : PixWidget (parent)
{
}

ClutterBar::~ClutterBar ()
{
}


void
ClutterBar::setPixmaps(Skin *skin)
{
	m_clutter_on = skin->getItem(Skin::CLUTTER_ON);
	m_clutter_off = skin->getItem(Skin::CLUTTER_OFF);

	m_clutter_o = skin->getItem(Skin::CLUTTER_O);
	m_clutter_a = skin->getItem(Skin::CLUTTER_A);
	m_clutter_i = skin->getItem(Skin::CLUTTER_I);
	m_clutter_d = skin->getItem(Skin::CLUTTER_D);
	m_clutter_v = skin->getItem(Skin::CLUTTER_V);

	m_pixmap = m_clutter_on;

	setMinimumSize (m_clutter_on.size ());
	setMaximumSize (m_clutter_on.size ());

	update();
}

void
ClutterBar::mousePressEvent (QMouseEvent *event)
{
	int y = event->pos().y();

	if (y > 2 && y < 11) {
		m_pixmap = m_clutter_o;
	} else if (y > 10 && y < 19) {
		m_pixmap = m_clutter_a;
	} else if (y > 18 && y < 27) {
		m_pixmap = m_clutter_i;
	} else if (y > 26 && y < 35) {
		m_pixmap = m_clutter_d;
	} else if (y > 34 && y < 43) {
		m_pixmap = m_clutter_v;
	}

	update();
}

void
ClutterBar::mouseReleaseEvent (QMouseEvent *event)
{
	m_pixmap = m_clutter_on;

	update();
}




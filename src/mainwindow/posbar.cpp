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

#include "posbar.h"
#include "Skin.h"

#include <QWidget>

PosBar::PosBar (QWidget *parent, uint bg, uint bnormal, uint bpressed)
	: PixmapSlider (parent)
{
	Skin *skin = Skin::getInstance ();
	m_slider_normal = bnormal;
	m_slider_pressed = bpressed;
	m_bg = bg;
	
	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps (Skin *)));

	setFixedSize (248, 10);

	setMinimum (0);
	setMaximum (0);

	hide ();
}

void
PosBar::setPixmaps (Skin *skin)
{
	QPixmap pixmap = skin->getItem (m_bg);
	setBackground (pixmap);
	setFixedSize (248, pixmap.height ());

	if ( !skin->getItem(m_slider_normal).isNull() &&
	     !skin->getItem(m_slider_pressed).isNull()) {
		setSliders (skin->getItem(m_slider_normal),
		            skin->getItem(m_slider_pressed));
	} else {
		setSliders (QPixmap (), QPixmap ());
	}

	update ();
}


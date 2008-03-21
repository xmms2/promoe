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

#include "VolumeSlider.h"

#include "Skin.h"

Slider::Slider (QWidget *parent, uint name_min, uint name_max,
                uint name_on, uint name_off, int min, int max) : PixmapSlider (parent)
{
	Skin *skin = Skin::getInstance();

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	setSliderOffset (1);

	m_name_min = name_min;
	m_name_max = name_max;
	m_name_on = name_on;
	m_name_off = name_off;

	setMinimum (min);
	setMaximum (max);

}

void
Slider::setPixmaps (Skin *skin)
{
	QPixmapList bglist;
	for (uint32_t i = m_name_min; i <= m_name_max; i++) {
		bglist.append (skin->getItem(i));
	}
	setBackground (bglist);

	if (!skin->getItem(m_name_on).isNull() && !skin->getItem(m_name_off).isNull()) {
		setSliders (skin->getItem(m_name_on), skin->getItem(m_name_off));
	} else {
		setSliders (QPixmap (), QPixmap ());
	}

	QPixmap pixmap_slider = skin->getItem(m_name_min);
	setFixedSize(pixmap_slider.size());
	resize(pixmap_slider.size());

	update();
}

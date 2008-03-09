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

#include "stereomono.h"
#include "Skin.h"

#include <QPainter>

StereoMono::StereoMono (QWidget *parent) : PixWidget (parent)
{
	setMinimumSize (56, 12);
	setMaximumSize (56, 12);

	m_pixmap = QPixmap (56, 12);
}

void
StereoMono::setPixmaps (Skin *skin)
{
	m_pixmap_stereo_on = skin->getItem (Skin::STEREO_1);
	m_pixmap_stereo_off = skin->getItem (Skin::STEREO_0);
	m_pixmap_mono_on = skin->getItem (Skin::MONO_1);
	m_pixmap_mono_off = skin->getItem (Skin::MONO_0);

	setStereoMono (m_stereo, m_mono);
}

void
StereoMono::drawPixmaps ()
{
	QPainter paint;
	paint.begin (&m_pixmap);

	paint.drawPixmap (QRect (0, 0, 27, 12),
					  m_pixmap_mono,
					  m_pixmap_mono.rect ());

	paint.drawPixmap (QRect (27, 0, 29, 12),
					  m_pixmap_stereo,
					  m_pixmap_stereo.rect ());
	paint.end ();

	update();
}

void
StereoMono::setStereoMono (bool stereo, bool mono)
{

	if (stereo) {
		m_pixmap_stereo = m_pixmap_stereo_on;
	} else {
		m_pixmap_stereo = m_pixmap_stereo_off;
	}

	if (mono) {
		m_pixmap_mono = m_pixmap_mono_on;
	} else {
		m_pixmap_mono = m_pixmap_mono_off;
	}

	m_stereo = stereo;
	m_mono = mono;

	drawPixmaps ();

}


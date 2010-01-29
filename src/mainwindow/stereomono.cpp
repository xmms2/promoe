/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2010 XMMS2 Team
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
#include "skinmanager.h"

#include <QPainter>

StereoMono::StereoMono (QWidget *parent)
           : QWidget (parent), m_stereo(0), m_mono(0)
{
	setFixedSize (56, 12);

	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps (Skin *)));
}

void
StereoMono::setPixmaps (Skin *skin)
{
	m_pixmap_stereo_on = skin->getItem (Skin::STEREO_1);
	m_pixmap_stereo_off = skin->getItem (Skin::STEREO_0);
	m_pixmap_mono_on = skin->getItem (Skin::MONO_1);
	m_pixmap_mono_off = skin->getItem (Skin::MONO_0);

	update ();
}

void
StereoMono::paintEvent (QPaintEvent *event)
{
	QPainter p (this);

	p.drawPixmap (0, 0, m_mono ? m_pixmap_mono_on : m_pixmap_mono_off);
	p.drawPixmap (27, 0, m_stereo ? m_pixmap_stereo_on : m_pixmap_stereo_off);
}

void
StereoMono::setStereoMono (bool stereo, bool mono)
{
	// if nothing changes, just return
	if ((m_stereo == stereo) && (m_mono == mono))
		return;

	m_stereo = stereo;
	m_mono = mono;

	update ();
}

#include "stereomono.moc"

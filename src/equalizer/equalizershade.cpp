/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2010-2016 XMMS2 Team
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

#include "equalizershade.h"

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include "skin.h"
#include "skinmanager.h"
#include "equalizerwindow.h"

EqualizerShade::EqualizerShade (EqualizerWindow *parent) : QWidget (parent),
	m_active (false)
{
	setFixedSize (275, 14);

	Skin *skin = SkinManager::instance ()->activeSkin ();

	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));
}

void
EqualizerShade::setPixmaps (Skin *skin)
{
	p_active = skin->getItem (Skin::EQ_WIN_SHADE_ACTIVE);
	p_inactive = skin->getItem (Skin::EQ_WIN_SHADE_INACTIVE);

	update ();
}

void
EqualizerShade::mouseDoubleClickEvent (QMouseEvent *event)
{
	EqualizerWindow *ew = dynamic_cast<EqualizerWindow *>(window ());
	ew->switchDisplay ();
}

void
EqualizerShade::paintEvent (QPaintEvent *event)
{
	QPainter p(this);
	if (m_active) {
		p.drawPixmap (rect (), p_active, p_active.rect ());
	} else {
		p.drawPixmap (rect (), p_inactive, p_inactive.rect ());
	}
}

void
EqualizerShade::setActive (bool active)
{
	m_active = active;

	update ();
}

#include "equalizershade.moc"

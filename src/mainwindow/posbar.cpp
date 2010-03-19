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

#include "posbar.h"

#include "skin.h"
#include "skinmanager.h"

#include <QWidget>

PosBar::PosBar (QWidget *parent) : PixmapSlider (parent)
{
	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps (Skin *)));

	setFixedSize (248, 10);

	setMinimum (0);
	setMaximum (0);

	hide ();
}

void
PosBar::setPixmaps (Skin *skin)
{
	QPixmap pixmap = skin->getItem (Skin::POSBAR);
	setBackground (pixmap);
	setFixedSize (248, pixmap.height ());

	setButton (skin->getButton (Skin::SLIDER_POSBAR_BUTTON));

	update ();
}

#include "posbar.moc"

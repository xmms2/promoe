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

#include "Button.h"

#include "Skin.h"

#include <QIcon>
#include <QPixmap>
#include <QMouseEvent>

Button::Button (QWidget *parent) : PixmapButton (parent)
{
	m_name_normal = 0;
	m_name_pressed = 0;

	Skin *skin = Skin::getInstance();
	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

}

Button::Button (QWidget *parent, uint normal, uint pressed, bool pls) : PixmapButton (parent)
{
	m_name_normal = normal;
	m_name_pressed = pressed;
	m_pls = pls;

	Skin *skin = Skin::getInstance();
	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));
}

Button::~Button ()
{
}

void
Button::setPixmaps(Skin *skin)
{
	if (!m_name_normal && !m_name_pressed) {
		return;
	}

	QIcon icon;
	QPixmap m_pixmap_normal, m_pixmap_pressed;

	if (m_pls) {
		m_pixmap_normal = skin->getPls (m_name_normal);
		m_pixmap_pressed = skin->getPls (m_name_pressed);
	} else {
		m_pixmap_normal = skin->getItem (m_name_normal);
		m_pixmap_pressed = skin->getItem (m_name_pressed);
	}
	icon.addPixmap(m_pixmap_normal);
	icon.addPixmap(m_pixmap_pressed, QIcon::Active);

	if (!m_pixmap_normal || m_pixmap_normal.isNull()) {
		qDebug ("OPPP! %d return NULL!", m_name_normal);
	}
	if (!m_pixmap_pressed || m_pixmap_pressed.isNull()) {
		qDebug ("OPPP! %d return NULL!", m_name_pressed);
	}

	setIcon(icon);
	setFixedSize (m_pixmap_normal.size ());
	update();
}

/*
 *
 * ToggleButton
 *
 */
ToggleButton::ToggleButton (QWidget *parent, uint on_normal, uint on_pressed,
							uint off_normal, uint off_pressed) :
					PixmapButton (parent)
{
	setCheckable (true);

	m_name_on_normal = on_normal;
	m_name_on_pressed = on_pressed;
	m_name_off_normal = off_normal;
	m_name_off_pressed = off_pressed;

	Skin *skin = Skin::getInstance();
	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));
}


void
ToggleButton::setPixmaps(Skin *skin)
{
	QIcon icon;
	QPixmap p = skin->getItem( m_name_on_normal );

	icon.addPixmap( p, QIcon::Normal, QIcon::On );
	icon.addPixmap( skin->getItem(m_name_on_pressed), QIcon::Active,
	                QIcon::On );
	icon.addPixmap( skin->getItem(m_name_off_normal), QIcon::Normal,
	                QIcon::Off );
	icon.addPixmap( skin->getItem(m_name_off_pressed), QIcon::Active,
	                QIcon::Off );
	setIcon(icon);

	setFixedSize (p.size ());

	update();
}


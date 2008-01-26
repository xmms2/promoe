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

#include "MainWindow.h"
#include "Button.h"

#include "Skin.h"

#include <QMouseEvent>

Button::Button (QWidget *parent) : PixWidget (parent)
{
	m_name_normal = 0;
	m_name_pressed = 0;
}

Button::Button (QWidget *parent, uint normal, uint pressed, bool pls) : PixWidget (parent)
{
	m_name_normal = normal;
	m_name_pressed = pressed;
	m_diffx = 0;
	m_diffy = 0;
	m_pls = pls;
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

	if (m_pls) {
		m_pixmap_normal = skin->getPls (m_name_normal);
		m_pixmap_pressed = skin->getPls (m_name_pressed);
	} else {
		m_pixmap_normal = skin->getItem (m_name_normal);
		m_pixmap_pressed = skin->getItem (m_name_pressed);
	}
	m_pixmap = m_pixmap_normal;

	if (!m_pixmap_normal || m_pixmap_normal.isNull()) {
		qDebug ("OPPP! %d return NULL!", m_name_normal);
	}
	if (!m_pixmap_pressed || m_pixmap_pressed.isNull()) {
		qDebug ("OPPP! %d return NULL!", m_name_pressed);
	}

	setMinimumSize (m_pixmap.size ());
	setMaximumSize (m_pixmap.size ());

	update();
}

void 
Button::mousePressEvent (QMouseEvent *event)
{
	m_pixmap = m_pixmap_pressed;

	m_diffx = event->pos().x();
	m_diffy = event->pos().y();

	update ();
}

void 
Button::mouseReleaseEvent (QMouseEvent *event)
{
	m_pixmap = m_pixmap_normal;
	update();
	emit clicked();
}

ToggleButton::ToggleButton (QWidget *parent, uint on_normal, uint on_pressed,
							uint off_normal, uint off_pressed) : 
					Button (parent, off_normal, off_pressed)
{

	m_name_on_normal = on_normal;
	m_name_on_pressed = on_pressed;
	m_name_off_normal = off_normal;
	m_name_off_pressed = off_pressed;
	m_toggled_on = false;

	connect (this, SIGNAL(clicked()), this, SLOT (toggleOn()));
}


void
ToggleButton::setPixmaps(Skin *skin)
{
	m_pixmap_on_normal = skin->getItem(m_name_on_normal);
	m_pixmap_on_pressed = skin->getItem(m_name_on_pressed);
	m_pixmap_off_normal = skin->getItem(m_name_off_normal);
	m_pixmap_off_pressed = skin->getItem(m_name_off_pressed);

	setCurrentPix ();

	setMinimumSize (m_pixmap.size ());
	setMaximumSize (m_pixmap.size ());

	update();
}

void
ToggleButton::setCurrentPix ()
{
	if (m_toggled_on) {
		m_pixmap_normal = m_pixmap_on_normal;
		m_pixmap_pressed = m_pixmap_on_pressed;
	} else {
		m_pixmap_normal = m_pixmap_off_normal;
		m_pixmap_pressed = m_pixmap_off_pressed;
	}

	m_pixmap = m_pixmap_normal;
}

void
ToggleButton::toggleOn ()
{
	if (!m_toggled_on) {
		m_toggled_on = true;
	} else {
		m_toggled_on = false;
	}

	setCurrentPix ();
	update ();
}

ToggleButton::~ToggleButton ()
{
}

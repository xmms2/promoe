/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
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
#include "skin.h"
#include "skinmanager.h"

#include <QMouseEvent>
#include <QPainter>

ClutterBar::ClutterBar (QWidget *parent) : QWidget (parent),
	m_val (Skin::CLUTTER_ON), m_checked_a (false), m_checked_d (false),
	m_show_always (true)
{
	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));
}

ClutterBar::~ClutterBar ()
{
}


void
ClutterBar::setPixmaps(Skin *skin)
{
	setPixmaps (skin->getPixmapList (Skin::PIXMAPS_CLUTTERBAR));
}

void
ClutterBar::setPixmaps (const QPixmapList &p)
{
	m_pixmaps = p;

	QPixmap pixmap = m_pixmaps.value (Skin::CLUTTER_ON);
	setFixedSize (pixmap.size ());

	update();
}

void
ClutterBar::mousePressEvent (QMouseEvent *event)
{
	m_val = posToVal (event->x (), event->y ());

	update();
}
void
ClutterBar::mouseMoveEvent (QMouseEvent *event)
{
	const int val = posToVal (event->x(), event->y());

	if (m_val != val) {
		m_val = val;

		update ();
	}
}

void
ClutterBar::mouseReleaseEvent (QMouseEvent *event)
{
	m_val = Skin::CLUTTER_ON;

	const int val = posToVal (event->x (), event->y());
	switch (val) {
		case Skin::CLUTTER_O:
			emit clicked_o ();
			break;
		case Skin::CLUTTER_A:
			m_checked_a = !m_checked_a;
			emit clicked_a (m_checked_a);
			break;
		case Skin::CLUTTER_I:
			emit clicked_i ();
			break;
		case Skin::CLUTTER_D:
			m_checked_d = !m_checked_d;
			emit clicked_d (m_checked_d);
			break;
		case Skin::CLUTTER_V:
			emit clicked_v ();
			break;
		default:
			break;
	}

	update();
}

void
ClutterBar::enterEvent (QEvent * event)
{
	update ();
}
void
ClutterBar::leaveEvent (QEvent * event)
{
	update ();
}

bool
ClutterBar::posInWidget (const int x, const int y) const
{
	return (x >= 0 && y >= 0 && x < width() && y < height());
}

int
ClutterBar::posToVal (const int x, const int y) const
{
	if (!posInWidget (x, y))
		return Skin::CLUTTER_ON;

	if (y <= 2) {
		return Skin::CLUTTER_ON;
	} else if (y <= 10) {
		return Skin::CLUTTER_O;
	} else if (y <= 18) {
		return Skin::CLUTTER_A;
	} else if (y <= 26) {
		return Skin::CLUTTER_I;
	} else if (y <= 34) {
		return Skin::CLUTTER_D;
	} else if (y <= 42) {
		return Skin::CLUTTER_V;
	}

	return Skin::CLUTTER_ON;
}

void
ClutterBar::paintEvent (QPaintEvent *event)
{
	QPainter paint (this);
	if (m_show_always || underMouse ()) {
		const QPixmap pix = m_pixmaps.value (m_val);
		if (pix.isNull ())
			return;
		paint.drawPixmap (rect (), pix, pix.rect ());
		if (m_checked_a) {
			const QPixmap pix_a = m_pixmaps.value (Skin::CLUTTER_A);
			const QRect rect(0, 10, pix_a.width (), 8);
			paint.drawPixmap (rect, pix_a, rect);
		}
		if (m_checked_d) {
			const QPixmap pix_d = m_pixmaps.value (Skin::CLUTTER_D);
			const QRect rect(0, 26, pix_d.width (), 8);
			paint.drawPixmap (rect, pix_d, rect);
		}
	} else {
		const QPixmap pix = m_pixmaps.value (Skin::CLUTTER_OFF);
		if (pix.isNull ())
			return;
		paint.drawPixmap (rect (), pix, pix.rect ());
	}
}

#include "clutterbar.moc"

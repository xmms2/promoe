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

#include "Skin.h"
#include "PixWidget.h"

#include <QPainter>
#include <QPixmap>

PixWidget::PixWidget (QWidget *parent) : QWidget (parent)
{
	Skin *skin = Skin::getInstance();
	m_pixmap = QPixmap(0,0);

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));
}


PixWidget::~PixWidget ()
{
}


void
PixWidget::setPixmaps(Skin *skin)
{
}

/*
 * Since almost every Widget I have here
 * is done by drawing self.pixmap onto self
 * we define this a generic PaintEvent handler
 */
void 
PixWidget::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull ()) {
		return;
	}

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (rect (), m_pixmap, m_pixmap.rect ());
	paint.end ();
}

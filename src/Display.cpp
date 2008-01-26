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
#include "Display.h"

#include "TitleBar.h"

#include "FileDialog.h"

SkinDisplay::SkinDisplay (QWidget *parent) : QWidget(parent)
{
	Skin *skin = Skin::getInstance ();

	m_mw = parent;

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps (Skin *)));
}

void
SkinDisplay::setPixmaps (Skin *skin)
{
}

void 
SkinDisplay::mousePressEvent (QMouseEvent *event)
{
	dynamic_cast<MainWindow *>(getMW())->raisePL ();
	m_diffX = event->x();
	m_diffY = event->y();
}

void 
SkinDisplay::mouseMoveEvent (QMouseEvent *event)
{
	m_mw->move (event->globalPos().x() - m_diffX,
				event->globalPos().y() - m_diffY);
}

void
SkinDisplay::enterEvent (QEvent *event)
{
	dynamic_cast<TitleBar *>(m_tbar)->setActive(true);
}

void
SkinDisplay::leaveEvent (QEvent *event)
{
	dynamic_cast<TitleBar *>(m_tbar)->setActive(false);
}

void 
SkinDisplay::paintEvent (QPaintEvent *event)
{
	QPainter(paint);

	paint.begin(this);
	paint.eraseRect(rect());
	paint.end();
}

void
SkinDisplay::fileOpen (void)
{
	FileDialog fd (this, "main_addfiles");
	QStringList files;

	files = fd.getFiles ();

	XMMSHandler &xmmsh = XMMSHandler::getInstance();
	if (files.count() > 0) {
		xmmsh.playlistClear ();
	}

	for (int i = 0; i < files.count(); i++) {
		xmmsh.playlistAddURL ("file://" + files.value(i));
	}
}


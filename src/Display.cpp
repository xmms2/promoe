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

#include "XMMSHandler.h"
#include "xcollection.h"

#include "mainwindow.h"
#include "Display.h"
#include "Skin.h"

#include "TitleBar.h"
#include "FileDialog.h"

#include <QPainter>

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
		xmmsh.xcollection ()->playlistClear ();
	}

	for (int i = 0; i < files.count(); i++) {
		xmmsh.playlistAddURL ("file://" + files.value(i));
	}
}


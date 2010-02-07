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

#include "xclient.h"
#include "xcollection.h"

#include "skindisplay.h"

#include "application.h"
#include "mainwindow.h"

#include "titlebar.h"
#include "FileDialog.h"


SkinDisplay::SkinDisplay (QWidget *parent) : QWidget(parent)
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
SkinDisplay::fileOpen (void)
{
	FileDialog fd (this, "main_addfiles");
	QStringList files;

	files = fd.getFiles ();

	const XClient *client = App->client ();
//	if (files.count() > 0) {
//		xmmsh.xcollection ()->playlistClear ();
//	}

	for (int i = 0; i < files.count(); i++) {
		client->xcollection ()->playlistAddUrl ("file://" + files.value(i));
	}
}

#include "skindisplay.moc"

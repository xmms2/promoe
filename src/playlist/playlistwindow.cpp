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

#include "playlistwindow.h"

#include <QResizeEvent>
#include <QPoint>
#include <QIcon>

#include "playlistwidget.h"
#include "playlistshade.h"

#include "mainwindow.h"

PlaylistWindow::PlaylistWindow (QWidget *parent) : BaseWindow (parent)
{
	setObjectName ("playlist"); // Name of the config group
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif

	QSettings s;
	setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);
	setAttribute (Qt::WA_DeleteOnClose);

	s.beginGroup (objectName ());
	if (!s.contains ("size")) {
		s.setValue ("size", QSize (275, 350));
	}
	resize (s.value ("size").toSize ());

	m_playlist = new PlaylistWidget (this);
	setCentralWidget (m_playlist);
	m_shaded = new PlaylistShade (this);

	if (!s.contains ("shaded"))
		s.setValue ("shaded", false);
	else
		s.setValue ("shaded", !s.value("shaded").toBool ());

	switchDisplay ();

	s.endGroup ();

	// FIXME: flickering
	//setSizeIncrement (25, 29);
}

void
PlaylistWindow::switchDisplay (void)
{
	QSettings s;

	s.beginGroup ("playlist");

	if (!s.value("shaded").toBool ()) {
		m_shaded->hide ();

		m_playlist->show ();
		s.setValue ("shaded", true);
		m_playlist->resize (s.value("size").toSize ());
		resize (s.value("size").toSize ());

	} else {
		m_playlist->hide ();

		m_shaded->show ();
		s.setValue ("shaded", false);
		m_shaded->resize (s.value("size").toSize ().width(), 14);
		resize (s.value("size").toSize ().width(), 14);
	}

	s.endGroup ();

	update ();

}

void
PlaylistWindow::resizeEvent (QResizeEvent *event)
{
	QSettings s;

	if (s.value("playlist/shaded").toBool ()) {
		s.setValue ("playlist/size", size ());
	}
	mw ()->attachWidgets ();
}

void
PlaylistWindow::enterEvent (QEvent *event)
{
	m_playlist->setActive (true);
	m_shaded->setActive (true);
}


void
PlaylistWindow::leaveEvent (QEvent *event)
{
	m_playlist->setActive (false);
	m_shaded->setActive (false);
}


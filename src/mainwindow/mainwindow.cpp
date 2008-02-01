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

#include "XMMSHandler.h"

#include "mainwindow.h"

#ifdef HAVE_SERVERBROWSER
#include "ServerBrowser.h"
#endif

#include "playlistwindow.h"
#include "maindisplay.h"
#include "shadeddisplay.h"
#include "equalizerwindow.h"
#include "Button.h"

#include <QSettings>
#include <QIcon>
#include <QPluginLoader>
#include <qplugin.h>

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
	QSettings s;

	setWindowFlags(Qt::FramelessWindowHint);
	setGeometry(100, 100, 275, 116);
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif

	if (!s.contains ("mainwindow/shaded"))
		setShaded (true);
	else
		setShaded (!isShaded ());

	/*
	 * initialize EqualizerWindow
	 */
	m_equalizer = new EqualizerWindow (this);
	if (!s.contains ("equalizer/pos")) {
		s.setValue ("equalizer/pos", QPoint (pos ().x (),
		                                     pos ().y ()+size ().height ()));
	}
	m_equalizer->move (s.value ("equalizer/pos").toPoint ());
	m_equalizer->setVisible (s.value ("equalizer/visible", false).toBool ());

	/*
	 * initialtize PlaylistWindow
	 */
	m_playlistwin = new PlaylistWindow (this);
	if (!s.contains ("playlist/pos")) {
		s.setValue ("playlist/pos", QPoint (pos ().x (),
		                                    pos ().y ()+size ().height ()));
	}
	m_playlistwin->move (s.value ("playlist/pos").toPoint ());
	// FIXME: this should be done in PlaylistWindow.
	// But promoe segfaults if done so
	m_playlistwin->setVisible (s.value("playlist/visible", false).toBool ());

	/*
	 * The MainDisplay is the mainwindow non-shaded mode
	 */
	m_display = new MainDisplay (this);
	setCentralWidget (m_display);
	m_display->show ();

	/*
	 * MainDisplay's shaded mode
	 */
	m_shaded = new ShadedDisplay (this);
	m_shaded->hide ();

	switchDisplay ();

	if (!s.contains ("mainwindow/pos"))
		s.setValue ("mainwindow/pos", QPoint (100, 100));

	move (s.value("mainwindow/pos").toPoint ());
}

MainWindow::~MainWindow ()
{
	delete Skin::getInstance ();
}

void
MainWindow::switchDisplay ()
{
	QSettings s;

	if (isShaded ()) {
		m_shaded->hide ();
		m_display->show ();
		resize (275, 116);
		setShaded (false);
	} else {
		m_display->hide ();
		m_shaded->show ();
		resize (275, 14);
		setShaded (true);
	}

	update ();

}

void
MainWindow::raisePL (void)
{ 
	m_playlistwin->raise (); 
}

void
MainWindow::moveEvent (QMoveEvent *event)
{
	QSettings s;
	s.setValue ("mainwindow/pos", pos ());
}


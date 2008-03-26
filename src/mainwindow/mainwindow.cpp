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

#include "mainwindow.h"

#ifdef HAVE_SERVERBROWSER
#include "ServerBrowser.h"
#endif

#include "playlistwindow.h"
#include "maindisplay.h"
#include "shadeddisplay.h"
#include "equalizerwindow.h"
#include "Skin.h"

#include <QSettings>
#include <QIcon>
#include <QPluginLoader>
#include <QMouseEvent>
#include <qplugin.h>

MainWindow::MainWindow (QWidget *parent) : BaseWindow (parent)
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

	attachWidgets ();
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
MainWindow::mouseMoveEvent (QMouseEvent *event)
{
	if ((event->buttons () & Qt::LeftButton) && !m_diff.isNull ()) {
	//	QWidgetList attached;
	//	QWidget *w;
	//	foreach (w, m_attachedWidgets.keys ()) {
	//		attached.append (w);
	//	}
		move (snapWindow (event->globalPos() - m_diff, m_attachedWidgets));
	}
}

void
MainWindow::moveEvent (QMoveEvent *event)
{
	QSettings s;
	s.setValue ("mainwindow/pos", pos ());

	// move all connected windows to their new position
	// at the moment connected windows can be m_playlistwin and m_equalizer
	if (!m_attachedWidgets.isEmpty ()) {
		QMap<BaseWindow *,QPoint>::const_iterator i
		                         = m_attachedWidgets.constBegin ();
		while (i != m_attachedWidgets.constEnd ()) {
			i.key()->move (pos () + i.value ());
			++i;
		}
	}
}

void
MainWindow::attachWidgets ()
{
	m_attachedWidgets.clear ();
	QList<BaseWindow *> widgets;
	QWidget *w;
	foreach (w, qApp->topLevelWidgets ()) {
		if (w == this) {
			continue;
		}
		if ((w->inherits ("BaseWindow")) && (w->isVisible ())) {
			widgets.append (qobject_cast<BaseWindow *> (w));
		}
	}
	// attach widgets that directly touch MainWindow
	BaseWindow *b;
	foreach (b, widgets) {
		if (b->touches (this)) {
			m_attachedWidgets[b] = b->pos ()- pos ();
		}
	}
	// now attach the windows, that indirectly touch mainwindow through an
	// attached window
	// widgets isn't modified, even if it might be more efficent, because
	// that might produce some ugly, hard to trace bugs (modifying the
	// base of an iterater while it is in use)
	if (!m_attachedWidgets.isEmpty ()) {
		bool found = false;
		BaseWindow *att;
		do {
			found = false;
			foreach (att, m_attachedWidgets.keys ()) {
				foreach (b, widgets) {
					if (m_attachedWidgets.contains (b)) {
						continue;
					}
					if (att->touches (b)) {
						m_attachedWidgets[b] = b->pos ()- pos ();
						found = true;
					}
				}
			}

		} while (found);
	}

}

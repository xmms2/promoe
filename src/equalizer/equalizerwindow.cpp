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

#include "equalizerwindow.h"

#include "QWidget"
#include <QSettings>

#include "mainwindow.h"
#include "equalizerwidget.h"
#include "equalizershade.h"

EqualizerWindow::EqualizerWindow (QWidget *parent) : BaseWindow (parent)
{
	setObjectName ("equalizer");

	setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);
	setAttribute (Qt::WA_DeleteOnClose);

	QSettings s;
	s.beginGroup (objectName ());

	m_equalizer = new EqualizerWidget (this);
	m_shaded = new EqualizerShade (this);
	setCentralWidget (m_equalizer);

	if (!s.contains ("shaded"))
		s.setValue ("shaded", false);
	else
		s.setValue ("shaded", !s.value("shaded").toBool ());
	switchDisplay ();

	s.endGroup ();
}

void
EqualizerWindow::activeWindowInEvent (QEvent *event)
{
	m_equalizer->setActive (true);
	m_shaded->setActive (true);
	BaseWindow::activeWindowInEvent (event);
}

void
EqualizerWindow::activeWindowOutEvent (QEvent *event)
{
	m_equalizer->setActive (false);
	m_shaded->setActive (false);
	BaseWindow::activeWindowOutEvent (event);
}

void
EqualizerWindow::switchDisplay (void)
{
	QSettings s;
	s.beginGroup (objectName ());

	if (s.value("shaded").toBool ()) {
		m_shaded->hide ();
		m_equalizer->show ();
		setFixedSize (m_equalizer->size ());
		s.setValue ("shaded", false);
	} else {
		m_equalizer->hide ();
		m_shaded->show ();
		setFixedSize (m_shaded->size ());
		s.setValue ("shaded", true);
    }

	s.endGroup ();
}

#include "equalizerwindow.moc"

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

#include "equalizerwindow.h"

#include "QWidget"
#include <QSettings>

#include "mainwindow.h"
#include "equalizerwidget.h"

EqualizerWindow::EqualizerWindow (QWidget *parent) : BaseWindow (parent)
{
	setObjectName ("equalizer");
	m_mw = dynamic_cast<MainWindow *>(parent);

	setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);
	setAttribute (Qt::WA_DeleteOnClose);

	m_equalizer = new EqualizerWidget (this);
	m_equalizer->show();

	setCentralWidget (m_equalizer);

	setFixedSize (275, 116);
}

void
EqualizerWindow::activeWindowInEvent (QEvent *event)
{
	m_equalizer->setActive (true);
	BaseWindow::activeWindowInEvent (event);
}

void
EqualizerWindow::activeWindowOutEvent (QEvent *event)
{
	m_equalizer->setActive (false);
	BaseWindow::activeWindowOutEvent (event);
}

void
EqualizerWindow::switchDisplay (void)
{
	qDebug("switchDisplay not implemented for equalizer");
}

#include "equalizerwindow.moc"

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

#include "equalizerwindow.h"

#include "QWidget"
#include <QSettings>
#include <QMouseEvent>
#include <QMoveEvent>

#include "mainwindow.h"
#include "equalizerwidget.h"

EqualizerWindow::EqualizerWindow (QWidget *parent) : QMainWindow (parent)
{
	m_mw = dynamic_cast<MainWindow *>(parent);

	setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);
	setAttribute (Qt::WA_DeleteOnClose);

	m_equalizer = new EqualizerWidget (this);
	m_equalizer->show();

	setCentralWidget (m_equalizer);

	setFixedSize (275, 116);
}

EqualizerWindow::~EqualizerWindow (void)
{
}

void
EqualizerWindow::setEnabled (void)
{
	qDebug ("test");
}


void
EqualizerWindow::mousePressEvent (QMouseEvent *event)
{
	m_diffx = event->pos().x();
	m_diffy = event->pos().y();
}

void
EqualizerWindow::mouseMoveEvent (QMouseEvent *event)
{
	move(event->globalPos().x() - m_diffx,
		 event->globalPos().y() - m_diffy);
}

void
EqualizerWindow::moveEvent (QMoveEvent *event)
{
	QSettings s;
	s.setValue ("equalizer/pos", pos ());
}

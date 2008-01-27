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
#include "TitleBar.h"
#include "equalizer.h"
#include "Button.h"
#include "VolumeSlider.h"

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

/**
 *
 * EqualizerWidget
 *
 */
EqualizerWidget::EqualizerWidget (QWidget *parent) : QWidget (parent)
{
	Skin *skin = Skin::getInstance ();

	connect (skin, SIGNAL(skinChanged(Skin *)),
	         this, SLOT(setPixmaps(Skin *)));

	m_enable = new ToggleButton(this, Skin::EQ_WIN_ON_0, Skin::EQ_WIN_ON_1,
	                            Skin::EQ_WIN_OFF_0, Skin::EQ_WIN_OFF_1);
	m_enable->move(14, 18);
	m_enable->setEnabled(false); // FIXME: needs to be implemented

	connect(m_enable, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_auto = new ToggleButton(this, Skin::EQ_WIN_AUTO_ON_0, Skin::EQ_WIN_AUTO_ON_1,
	                          Skin::EQ_WIN_AUTO_OFF_0, Skin::EQ_WIN_AUTO_OFF_1);
	m_auto->move(39, 18);
	m_auto->setEnabled(false); // FIXME: needs to be implemented

	connect(m_auto, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_preset = new Button(this, Skin::EQ_WIN_PRESET_0, Skin::EQ_WIN_PRESET_1);
	m_preset->move(217, 18);
	m_preset->setEnabled(false); // FIXME: needs to be implemented

	connect(m_preset, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_preamp = new Slider(this, Skin::EQ_WIN_BAR_POS_0, Skin::EQ_WIN_BAR_POS_27,
	                      Skin::EQ_WIN_BAR_BTN_0, Skin::EQ_WIN_BAR_BTN_1, -20, 20);
	m_preamp->move(21, 38);

	for (int i=0; i < 10; i++) {
		m_bands[i] = new Slider(this, Skin::EQ_WIN_BAR_POS_0, Skin::EQ_WIN_BAR_POS_27,
		                        Skin::EQ_WIN_BAR_BTN_0, Skin::EQ_WIN_BAR_BTN_1, -20, 20);
		m_bands[i]->move(78+i*18, 38);
	}
}

EqualizerWidget::~EqualizerWidget (void)
{
}

void
EqualizerWidget::setPixmaps (Skin *skin)
{
	m_pixmap = skin->getItem(Skin::EQ_WIN_BG);
	m_graph = skin->getItem(Skin::EQ_WIN_GRAPH_BG);

	setMinimumSize (m_pixmap.size ());
	setMaximumSize (m_pixmap.size ());

	update();
}

void 
EqualizerWidget::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull ()) {
		return;
	}
	QPainter(paint);
	QRect r;

	paint.begin(this);
	
	paint.drawPixmap(rect(), m_pixmap, m_pixmap.rect());
	r.setRect(86, 17, 113, 19);
	paint.drawPixmap(r, m_graph, m_graph.rect());
	
	paint.end();
}

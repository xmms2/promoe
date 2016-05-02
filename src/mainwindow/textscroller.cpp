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

#include <application.h>
#include "skin.h"
#include "skindisplay.h"
#include "skinmanager.h"
#include "textscroller.h"

#include <QBrush>
#include <QFont>
#include <QMouseEvent>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QSettings>
#include <QTimer>

#include "promoe_config.h"

TextScroller::TextScroller (QWidget *parent, uint w,
                            uint h, const QString &name) : QWidget (parent)
{
	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	QSettings s;

	s.beginGroup ("display_" + name);

	if (!s.contains("scroll"))
		s.setValue("scroll", true);

	if (!s.contains("fontsize"))
		s.setValue("fontsize", 8);

	if (!s.contains("ttf"))
		s.setValue("ttf", true);

	m_name = name;
	m_x_off = 0;
	m_drag_off = 0;
	m_fontsize = s.value ("fontsize").toInt ();
	m_ttf = s.value ("ttf").toBool ();
	m_text = "Promoe " PROMOE_VERSION;
	m_scroll = s.value ("scroll").toBool ();
	m_dragtext = false;

	s.endGroup ();

	setFixedSize(w, h);

	m_timer = new QTimer (this);
	m_timer->setInterval (40);
	connect (m_timer, SIGNAL (timeout()), this, SLOT (addOffset ()));
	connect (App, SIGNAL (settingsChanged ()),
	         this, SLOT (settingsChanged ()));
}

void
TextScroller::settingsChanged (void)
{
	QSettings s;
	s.beginGroup ("display_" + m_name);
	m_fontsize = s.value ("fontsize").toInt ();
	m_ttf = s.value ("ttf").toBool ();

	if (m_scroll != s.value ("scroll").toBool ()) {
		m_x_off = 0;
	}

	m_scroll = s.value ("scroll").toBool ();
	s.endGroup ();

	drawText ();
	update ();
}

void
TextScroller::setPixmaps (Skin *skin)
{
	QPalette pal = palette ();
	QBrush b = QBrush (skin->getItem (Skin::TEXTBG));
	pal.setBrush (QPalette::Window, b);
	setPalette (pal);

	drawText ();
	update();
}

void
TextScroller::setText (QString text)
{
	m_text = text;

	drawText ();
}

void
TextScroller::addOffset ()
{
	if (m_x_off < m_pixmap.width()) {
		m_x_off ++;
	} else {
		m_x_off = 0;
	}

	update ();
}

void
TextScroller::drawText ()
{
	if (m_ttf) {
		drawQtFont (m_text);
	} else {
		drawBitmapFont (m_text);
	}
	updateScrolling ();

	if (m_pixmap.width () <= width ())
		m_dragtext = false;

	// take care that the text doesn't jump after resetting it's offset
	// if we were still dragging it on a song change
	if (m_dragtext)
		m_drag_off -= m_x_off;

	m_x_off = 0;
	update ();
}

void
TextScroller::drawBitmapFont (QString text)
{
	Skin *skin = SkinManager::instance ()->activeSkin ();

	int w = text.length() * 5;
	QString temp = text.toLower ();

	if (w > width ()) {
		temp += QString ("  ***  ");
		m_pixmap = QPixmap (w + 7*5, 6);
	} else {
		m_pixmap = QPixmap (width (), 6);
	}
	QByteArray temp2 = temp.toLatin1();
	const char *t = temp2.data();

	QPainter (paint);
	paint.begin (&m_pixmap);

	paint.drawPixmap (m_pixmap.rect (), skin->getItem (Skin::TEXTBG));

	for (uint i = 0; i < strlen (t); i++) {
		QPixmap p = skin->getLetter (t[i]);
		if (p.isNull ()) {
			p = skin->getLetter(' ');
		}

		paint.drawPixmap (QRect ((i * 5), 0, 5, 6),
						  p, p.rect());
	}

	paint.end();
}

void
TextScroller::drawQtFont (QString text)
{
	Skin *skin = SkinManager::instance ()->activeSkin ();

	QFont font(skin->getPLeditValue ("font"));
	font.setPixelSize (m_fontsize);

	QFontMetrics fM(font);
	QRect rect = fM.boundingRect (text);

	QString (temp) = text;

	if (rect.width() > width ()) {
		temp += QString ("  ***  ");
		int w = fM.width (temp);

		m_pixmap = QPixmap (w, height ());
	} else {
		m_pixmap = QPixmap (size ());
	}

	QPainter paint;
	paint.begin (&m_pixmap);
	paint.drawPixmap (m_pixmap.rect (),
	                  skin->getItem (Skin::TEXTBG),
	                  skin->getItem (Skin::TEXTBG).rect ());

	paint.setFont (font);
	QColor c;
	c.setNamedColor (skin->getPLeditValue ("normal"));
	paint.setPen (c);
	paint.drawText (m_pixmap.rect (),
					Qt::AlignLeft | Qt::AlignVCenter,
					temp);
	paint.end ();
}

void 
TextScroller::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull ()) {
		return;
	}

	// A pixmap font is only 6 pixels high and should be centered vertically
	// for a QFont h_offset is 0
	int h_offset = (height () - m_pixmap.height ()) /2;

	int left_width = qMin (m_pixmap.width() - m_x_off, width ());

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (QPoint (0, h_offset),
	                  m_pixmap,
	                  QRect (m_x_off, 0, left_width, m_pixmap.height ()));
	if (left_width < width ()) {
		paint.drawPixmap (left_width, h_offset, m_pixmap);
	}
	paint.end ();
}

inline void
TextScroller::updateScrolling ()
{
	if (m_scroll && !m_dragtext && (m_pixmap.width () > width ())) {
		m_timer->start ();
	} else {
		m_timer->stop ();
	}
}

void
TextScroller::mousePressEvent (QMouseEvent *event)
{
	if (event->button () != Qt::LeftButton) {
		event->ignore ();
		return;
	}

	if (m_pixmap.width () <= width ()) {
		// don't use event->ignore here!
		return;
	}

	// calculate the offset relative to m_pixmap
	// if the offset would be saved relative to the widget another
	// helpervariable would become necessary to save m_x_off
	// m_drag_off can be bigger than the width of m_pixmap but that is no
	// problem as we use the remainder operator in the calculation results
	m_drag_off = m_x_off + event->x();

	m_dragtext = true;
	updateScrolling ();
}

void
TextScroller::mouseReleaseEvent (QMouseEvent *event)
{
	if (event->button () != Qt::LeftButton) {
		event->ignore ();
		return;
	}

	m_drag_off = 0;

	m_dragtext = false;
	updateScrolling ();
}

void
TextScroller::mouseMoveEvent (QMouseEvent *event)
{
	if (!m_dragtext) {
		event->ignore ();
		return;
	}

	m_x_off = ( m_drag_off - event->x()) % m_pixmap.width ();
	// make sure we have a positive value
	if (m_x_off < 0)
		m_x_off += m_pixmap.width ();

	update ();
}

#include "textscroller.moc"

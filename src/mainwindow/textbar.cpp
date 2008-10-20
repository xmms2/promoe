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

#include "skindisplay.h"
#include "textbar.h"
#include "Skin.h"

#include <QBrush>
#include <QFont>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QSettings>
#include <QTimer>

TextScroller::TextScroller (QWidget *parent, uint w,
                            uint h, const QString &name) :
	QWidget (parent)
{
	Skin *skin = Skin::getInstance ();

	connect (skin, SIGNAL (skinChanged (Skin *)),
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
	m_h = h;
	m_w = w;
	m_x_off = 0;
	m_x2_off = 0;
	m_fontsize = s.value ("fontsize").toInt ();
	m_ttf = s.value ("ttf").toBool ();
	m_text = "Promoe " PROMOE_VERSION;
	m_scroll = s.value ("scroll").toBool ();

	s.endGroup ();
	
	setMinimumSize(m_w + 2, m_h);
	setMaximumSize(m_w + 2, m_h);

	m_timer = new QTimer (this);
	connect (m_timer, SIGNAL (timeout()), this, SLOT (addOffset ()));
	//connect (xmmsh, SIGNAL (settingsSaved ()), this, SLOT (settingsSaved ()));
}

void
TextScroller::settingsSaved (void)
{
	QSettings s;
	s.beginGroup ("display_" + m_name);
	m_fontsize = s.value ("fontsize").toInt ();
	m_ttf = s.value ("ttf").toBool ();

	if (m_scroll != s.value ("scroll").toBool ()) {
		m_x_off = 0;
		m_x2_off = 0;
	}

	m_scroll = s.value ("scroll").toBool ();
	s.endGroup ();

	setText (m_text);
	update ();
}

void
TextScroller::setPixmaps (Skin *skin)
{
	QPalette pal = palette ();
	QBrush b = QBrush (skin->getItem (Skin::TEXTBG));
	pal.setBrush (QPalette::Window, b);
	setPalette (pal);

	setText (m_text);
	update();
}

void 
TextScroller::addOffset ()
{
	if (m_x2_off > 0) {
		m_x2_off --;
	} else if (m_x_off < m_pixmap.size().width()) {
		m_x_off ++;
	} else {
		m_x_off = 0;
		m_x2_off = 0;
	}

	update ();
	m_timer->start (40);
}

void
TextScroller::setText (QString text)
{
	m_text = text;

	if (m_ttf) {
		drawQtFont (text);
	} else {
		drawBitmapFont (text);
	}
	m_x_off = 0;
	m_x2_off = 0;
	update ();
}

void
TextScroller::drawBitmapFont (QString text)
{
	Skin *skin = Skin::getInstance ();

	int width = text.length() * 6;
	QString temp = text.toLower ();

	if (width > m_w) {
		temp += QString::fromAscii ("  --  ");
		m_pixmap = QPixmap (width + 6*6, m_h);

		if (m_scroll) {
			m_timer->start (40);
		} else {
			m_timer->stop ();
		}
	} else {
		m_pixmap = QPixmap (m_w, m_h);
		m_timer->stop ();
	}
	QByteArray temp2 = temp.toLatin1();
	const char *t = temp2.data();

	QPainter (paint);

	paint.begin (&m_pixmap);

	paint.drawPixmap (m_pixmap.rect (),
	                  skin->getItem (Skin::TEXTBG),
	                  skin->getItem (Skin::TEXTBG).rect ());

	for (uint i = 0; i < strlen (t); i++) {
		QPixmap p = skin->getLetter (t[i]);
		if (p.isNull ()) {
			p = skin->getLetter(' ');
		}

		paint.drawPixmap (QRect ((i * 6), 0, 4, 6),
						  p, p.rect());
	}

	paint.end();

}

void
TextScroller::drawQtFont (QString text)
{
	Skin *skin = Skin::getInstance ();

	QFont font(skin->getPLeditValue ("font"));
	font.setPixelSize (m_fontsize);

	QFontMetrics fM(font);
	QRect rect = fM.boundingRect (text);

	QString (temp) = text;

	if (rect.width() > m_w) {
		temp += QString::fromAscii ("  --  ");
		QRect rect = fM.boundingRect (temp);

		m_pixmap = QPixmap (rect.width(), m_h);

		if (m_scroll) {
			m_timer->start (40);
		} else {
			m_timer->stop ();
		}

	} else {
		m_pixmap = QPixmap (m_w, m_h);
		m_timer->stop ();
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
	int pad = 0;

	if (m_pixmap.isNull ()) {
		return;
	}

	int w2 = m_pixmap.size().width() - m_x_off;
	if (w2 < m_w) {
		pad = m_w - w2;
	}

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (QRect (m_x2_off, 0, m_w - pad, m_h),
	                  m_pixmap,
	                  QRect (m_x_off, 0, m_w - pad, m_h));
	if (pad) {
		paint.drawPixmap (QRect (m_w - pad, 0, pad, m_h),
		                  m_pixmap,
		                  QRect (0, 0, pad, m_h));
	}
	paint.end ();
}

TextScroller::~TextScroller ()
{
}


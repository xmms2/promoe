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

#include "xclient.h"
#include "xclientcache.h"

#include "application.h"
#include "pixmapbutton.h"
#include "playlistshade.h"
#include "playlistwindow.h"
#include "skin.h"
#include "skinmanager.h"

#include <QSettings>
#include <QPainter>
#include <QPoint>

#include "promoe_config.h"

PlaylistShade::PlaylistShade (PlaylistWindow *parent) : QWidget (parent)
{
	QSettings s;
	const XClient *client = App->client ();

	if (!s.contains ("playlist/shadedsize"))
		s.setValue ("playlist/shadedsize", 8);

	Skin *skin = SkinManager::instance ()->activeSkin ();
	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	m_closebtn = new PixmapButton (this);
	m_closebtn->resize (skin->getSize (Skin::BUTTON_PLS_CLOSE));
	connect (m_closebtn, SIGNAL (clicked ()), parent, SLOT (hide ()));

	m_unshadebtn = new PixmapButton (this);
	m_unshadebtn->resize (skin->getSize (Skin::BUTTON_PLS_SHADED_UNSHADE));
	connect (m_unshadebtn, SIGNAL (clicked ()),
	         parent, SLOT (switchDisplay ()));

	connect (client->cache (), SIGNAL (activeEntryChanged (QVariantHash)),
	         this, SLOT (setMediainfo (QVariantHash)));

	connect (App, SIGNAL(settingsChanged ()), 
			 this, SLOT(settingsChanged ()));

	m_text = "Promoe " PROMOE_VERSION  " - A very neat XMMS2 client";
	setMinimumSize (275, 14);
}

void
PlaylistShade::settingsChanged ()
{
	QSettings s;

	m_font.setPixelSize (s.value ("playlist/shadedsize").toInt ());
	update ();
}

void
PlaylistShade::setMediainfo (QVariantHash info)
{
	QString n;
	if (info.contains ("artist") && info.contains ("album") &&
	    info.contains ("title")) {
		n = info["artist"].toString () + " - "
		  + info["album"].toString () + " - "
		  + info["title"].toString ();
	} else {
		n = info["url"].toString ();
	}
	m_text = (n);

	update ();
}

void
PlaylistShade::resizeEvent (QResizeEvent *event)
{
	Skin *skin = SkinManager::instance ()->activeSkin ();

	QPoint p = skin->getPos (Skin::BUTTON_PLS_CLOSE);
	m_closebtn->move (p.x () + width (), p.y());

	p = skin->getPos (Skin::BUTTON_PLS_SHADED_UNSHADE);
	m_unshadebtn->move (p.x () + width (), p.y());
}

void
PlaylistShade::setPixmaps (Skin *skin)
{
	QSettings s;

	m_closebtn->setPixmaps (skin->getButton (Skin::BUTTON_PLS_SHADED_CLOSE));
	m_unshadebtn->setPixmaps (skin->getButton (Skin::BUTTON_PLS_SHADED_UNSHADE));

	m_pixmap_le = skin->getPls (Skin::PLS_WS_LE_0);
	m_pixmap_re_0 = skin->getPls (Skin::PLS_WS_RE_0);
	m_pixmap_re_1 = skin->getPls (Skin::PLS_WS_RE_1);
	m_pixmap_mid = skin->getPls (Skin::PLS_WS_MID_0);
	m_pixmap_re = m_pixmap_re_0;

	m_font = QFont (skin->getPLeditValue ("font"));
	m_font.setPixelSize (s.value ("playlist/shadedsize").toInt ());
	m_color.setNamedColor (skin->getPLeditValue ("normal"));

	update ();

}

void
PlaylistShade::setActive (bool b)
{
	if (b) {
		m_pixmap_re = m_pixmap_re_0;
	} else {
		m_pixmap_re = m_pixmap_re_1;
	}

	update ();
}

void
PlaylistShade::mouseDoubleClickEvent (QMouseEvent *event)
{
	PlaylistWindow *pw = dynamic_cast<PlaylistWindow*>(window ());
	pw->switchDisplay ();
}

void
PlaylistShade::paintEvent (QPaintEvent *event)
{
	QRect r;

	QPainter p;
	p.begin (this);
	r.setRect (0, 0, m_pixmap_le.size().width(), m_pixmap_le.size().height());
	p.drawPixmap (r, m_pixmap_le, m_pixmap_le.rect());
	int s = size().width() - m_pixmap_le.size().width() - m_pixmap_re.size().width();
	r.setRect (m_pixmap_le.size().width(), 0, s, m_pixmap_mid.size().height());
	p.drawPixmap (r, m_pixmap_mid, m_pixmap_mid.rect ());
	r.setRect (size().width() - m_pixmap_re.size().width(), 0, m_pixmap_re.size().width(),
			   m_pixmap_re.size().height());
	p.drawPixmap (r, m_pixmap_re, m_pixmap_re.rect ());

	/* Text */
	p.setFont (m_font);
	p.setPen (m_color);

	p.drawText (QRect (5, 4, size().width()-25, 7),
				Qt::AlignLeft | Qt::AlignVCenter,
				m_text);

	p.end ();

}

#include "playlistshade.moc"

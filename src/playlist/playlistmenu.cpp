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

#include "playlistmenu.h"
#include "Skin.h"
#include "skinmanager.h"

#include <QMouseEvent>
#include <QWidget>
#include <QPoint>
#include <QPainter>


PlaylistMenuBar::PlaylistMenuBar (QWidget *parent, uint id) : QWidget (parent)
{
	m_id = id;
	m_pixmap = QPixmap (0,0);

	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps (Skin *)));
}

void
PlaylistMenuBar::paintEvent (QPaintEvent *event)
{
	QPainter p;
	p.begin (this);
	p.drawPixmap (rect (), m_pixmap, m_pixmap.rect ());
	p.end ();
}


PlaylistMenuButton::PlaylistMenuButton (PlaylistMenu *menu, 
										uint pix1, uint pix2) : QWidget (menu)
{
	m_pixid1 = pix1;
	m_pixid2 = pix2;
	menu->addButton (this);
	m_menu = menu;
	setFixedSize (22, 18);
}

PlaylistMenuButton::~PlaylistMenuButton ()
{
	m_menu->rmButton (this);
}

void
PlaylistMenuButton::paintEvent (QPaintEvent *event)
{
	QPainter p;
	p.begin (this);
	p.drawPixmap (rect (), m_pixmap, m_pixmap.rect ());
	p.end ();
}

void
PlaylistMenuButton::setPixmaps (Skin *skin)
{
	m_pixmap1 = skin->getPls (m_pixid1);
	m_pixmap2 = skin->getPls (m_pixid2);

	m_pixmap = m_pixmap1;
}


/*
 * PlaylistMenu
 */
PlaylistMenu::PlaylistMenu (QWidget *parent, uint pix,
							uint decoration) : QWidget (parent)
{
	setFixedSize (25, 18);

	m_expanded = new QWidget (parent);
	m_expanded->hide ();

	m_decbar = new PlaylistMenuBar (m_expanded, decoration);
	m_decbar->move (0, 0);

	m_pixid = pix;
	m_pixmap = QPixmap (0,0);

	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps (Skin *)));
}

void
PlaylistMenu::paintEvent (QPaintEvent *event)
{
	QPainter p;
	p.begin (this);
	p.drawPixmap (rect (), m_pixmap, m_pixmap.rect ());
	p.end ();
}

void
PlaylistMenu::addButton (PlaylistMenuButton *b)
{
	int i = m_items.count ();
	/* black magic, don't try this at home kids */
	b->setParent (m_expanded);
	b->move (3, i * 18);
	m_items.append (b);
	i = m_items.count ();
	m_decbar->resize (3, i * 18);
	m_expanded->resize (25, i * 18);

}

void
PlaylistMenu::rmButton (PlaylistMenuButton *b)
{
	m_items.removeAll (b);
}

void
PlaylistMenu::mousePressEvent (QMouseEvent *event)
{
	PlaylistMenuButton *b = m_items.last ();
	b->setOver ();

	QWidget *w = dynamic_cast<QWidget*>(parent ());

	m_expanded->move (pos ().x (), 
					  w->height() - m_expanded->height() - 12);

	m_expanded->show ();

}

void
PlaylistMenu::mouseMoveEvent (QMouseEvent *event)
{
	int my_y = event->y() + (m_items.count () - 1) * 18;

	for (int i = 0; i < m_items.count (); i ++) {
		PlaylistMenuButton *b = m_items.value (i);

		if (my_y > b->pos().y() &&
			my_y < b->pos().y() + 18) {
			b->setOver ();
		} else {
			b->setNoOver ();
		}

	}
	m_expanded->update ();

}

void
PlaylistMenu::mouseReleaseEvent (QMouseEvent *event)
{
	int my_y = event->y() + (m_items.count () - 1) * 18;

	for (int i = 0; i < m_items.count (); i ++) {
		PlaylistMenuButton *b = m_items.value (i);

		if (my_y > b->pos().y() &&
			my_y < b->pos().y() + 18) {

			b->clicked ();
		}
	}
	m_expanded->hide ();

	dynamic_cast<QWidget*>(parent ())->update ();
}

void
PlaylistMenu::setPixmaps (Skin *skin)
{
	for (int i = 0; i < m_items.count (); i++) {
		PlaylistMenuButton *b = m_items.value (i);
		if (b) {
			b->setPixmaps (skin);
		}
	}

	m_pixmap = skin->getPls (m_pixid);
	m_dpixmap = skin->getPls (m_dec);

	update ();
}

#include "playlistmenu.moc"

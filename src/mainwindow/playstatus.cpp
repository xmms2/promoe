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

#include "playstatus.h"
#include "Skin.h"
#include "skinmanager.h"

#include <QPainter>

PlayStatus::PlayStatus (QWidget *parent) : QWidget (parent)
{
	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps (Skin *)));

	setFixedSize(11, 9);

	m_status = Xmms::Playback::STOPPED;
}

void
PlayStatus::setPixmaps (Skin *skin)
{
	m_pixmap_play = skin->getItem (Skin::PIC_PLAY);
	m_pixmap_pause = skin->getItem (Skin::PIC_PAUSE);
	m_pixmap_stop = skin->getItem (Skin::PIC_STOP);

	setFixedSize(11, m_pixmap_play.height ());
	update ();
}

void
PlayStatus::setStatus (Xmms::Playback::Status status)
{
	if (m_status == status)
		return;

	m_status = status;
	update ();
}

void
PlayStatus::paintEvent (QPaintEvent *event)
{
	QPixmap pixmap;
	using Xmms::Playback;
	switch (m_status) {
		case Playback::STOPPED:
			pixmap = m_pixmap_stop;
			break;
		case Playback::PLAYING:
			pixmap = m_pixmap_play;
			break;
		case Playback::PAUSED:
			pixmap = m_pixmap_pause;
			break;
		default:
			qWarning ("Unhandled playback status in PlayStatus");
			break;
	}

	QPainter p;
	p.begin (this);
	p.drawPixmap (rect (), pixmap);
	p.end ();
}

#include "playstatus.moc"

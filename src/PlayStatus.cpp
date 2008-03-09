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

#include <xmmsclient/xmmsclient++.h>
#include "PlayStatus.h"
#include "Skin.h"

PlayStatus::PlayStatus (QWidget *parent) : PixWidget (parent)
{
	setMinimumSize(11, 9);
	setMaximumSize(11, 9);

	m_status = Xmms::Playback::STOPPED;

	connect (&XMMSHandler::getInstance (),
	         SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(setStatus(Xmms::Playback::Status)));
}

void
PlayStatus::setPixmaps (Skin *skin)
{
	m_pixmap_play = skin->getItem (Skin::PIC_PLAY);
	m_pixmap_pause = skin->getItem (Skin::PIC_PAUSE);
	m_pixmap_stop = skin->getItem (Skin::PIC_STOP);

	setStatus (m_status);
}

void
PlayStatus::setStatus (Xmms::Playback::Status status)
{
	using Xmms::Playback;
	if (status == Playback::STOPPED) {
		m_pixmap = m_pixmap_stop;
	} else if (status == Playback::PLAYING) {
		m_pixmap = m_pixmap_play;
	} else if (status == Playback::PAUSED) {
		m_pixmap = m_pixmap_pause;
	}

	update ();
}


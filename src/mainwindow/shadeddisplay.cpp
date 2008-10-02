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
#include "XMMSHandler.h"
#include "xclientcache.h"
#include "xplayback.h"

#include "shadeddisplay.h"
#include "titlebar.h"
#include "pixmapbutton.h"

#include "timedisplay.h"
#include "textbar.h"
#include "Skin.h"
#include "mainwindow.h"

ShadedDisplay::ShadedDisplay (QWidget *parent) : SkinDisplay (parent)
{
	XMMSHandler &client = XMMSHandler::getInstance ();

	setFixedSize (275, 14);

	m_tbar = new TitleBar(this, true);
	m_tbar->move (0, 0);

	m_time = new SmallTimeDisplay (this);
	m_time->move (130, 4);
	MainWindow *mw = dynamic_cast<MainWindow *>(parent);
	connect (m_time, SIGNAL(clicked()), mw, SLOT(toggleTime()));

	m_title = new TextScroller (this, 39, 7, "shaded");
	m_title->move (79, 4);

	m_prev = new PixmapButton (this);
	m_prev->move(169, 4);
	m_prev->resize (8, 7);
	connect (m_prev, SIGNAL(clicked()), client.xplayback (), SLOT(prev ()));

	m_play = new PixmapButton (this);
	m_play->move(177, 4);
	m_play->resize (10, 7);
	connect (m_play, SIGNAL(clicked()), client.xplayback (), SLOT(play ()));

	m_pause = new PixmapButton (this);
	m_pause->move(187, 4);
	m_pause->resize (10, 7);
	connect (m_pause, SIGNAL(clicked()),
	         client.xplayback (), SLOT(toggle_pause ()));

	m_stop = new PixmapButton (this);
	m_stop->move(197, 4);
	m_stop->resize (9, 7);
	connect (m_stop, SIGNAL(clicked()), client.xplayback (), SLOT(stop ()));

	m_next = new PixmapButton (this);
	m_next->move(206, 4);
	m_next->resize (8, 7);
	connect (m_next, SIGNAL(clicked()), client.xplayback (), SLOT(next ()));

	m_eject = new PixmapButton (this);
	m_eject->move(216, 4);
	m_eject->resize (9, 7);
	connect (m_eject, SIGNAL(clicked()), this, SLOT(fileOpen()));

	connect (client.xplayback (), SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(setStatus(Xmms::Playback::Status)));
	connect (client.cache (), SIGNAL (playtime (uint32_t)),
	         this, SLOT ( setPlaytime(uint32_t)));
	connect (client.cache (), SIGNAL (activeEntryChanged (QVariantHash)),
	         this, SLOT (setMediainfo (QVariantHash)));
}

void
ShadedDisplay::setPixmaps (Skin *skin)
{
	m_time->setPixmaps (skin->getSmallNumbers ());
}

void
ShadedDisplay::setMediainfo (QVariantHash info)
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

	if (info.contains ("duration")) {
		m_duration = info["duration"].toInt ();
	} else {
		m_duration = 0;
	}
	m_title->setText (n);
}

void
ShadedDisplay::setStatus (Xmms::Playback::Status status)
{
	if (status == Xmms::Playback::STOPPED) {
		//m_number->setNumber (0, 2);
		//nm_number2->setNumber (0, 2);
	}
}

void
ShadedDisplay::setPlaytime (uint32_t time)
{
	int32_t showtime;
	if (dynamic_cast<MainWindow *>(m_mw)->isTimemodeReverse()) {
		showtime = (time/1000 - m_duration/1000);
	} else {
		showtime = time/1000;
	}
	m_time->setTime (showtime);
}



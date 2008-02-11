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

#include <xmmsclient/xmmsclient++.h>
#include "XMMSHandler.h"
#include "xclientcache.h"
#include "xplayback.h"

#include "shadeddisplay.h"
#include "TitleBar.h"
#include "Button.h"

#include "SmallNumberDisplay.h"
#include "TextBar.h"

ShadedDisplay::ShadedDisplay (QWidget *parent) : SkinDisplay (parent)
{
	XMMSHandler &client = XMMSHandler::getInstance ();

	setMinimumSize (275, 14);
	setMaximumSize (275, 14);

	m_tbar = new TitleBar(this, true);
	m_tbar->move (0, 0);

	m_number = new SmallNumberDisplay (this, 10);
	m_number->move (135, 4);
	m_number->setNumber (0, 2);

	m_number2 = new SmallNumberDisplay (this, 10);
	m_number2->move (147, 4);
	m_number2->setNumber (0, 2);

	m_title = new TextScroller (this, 39, 7, "shaded");
	m_title->move (79, 4);

	m_prev = new Button (this);
	m_prev->move(169, 4);
	m_prev->resize (8, 7);
	connect (m_prev, SIGNAL(clicked()), client.xplayback (), SLOT(prev ()));
	
	m_play = new Button (this);
	m_play->move(177, 4);
	m_play->resize (10, 7);
	connect (m_play, SIGNAL(clicked()), client.xplayback (), SLOT(play ()));

	m_pause = new Button (this);
	m_pause->move(187, 4);
	m_pause->resize (10, 7);
	connect (m_pause, SIGNAL(clicked()), client.xplayback (), SLOT(pause ()));

	m_stop = new Button (this);
	m_stop->move(197, 4);
	m_stop->resize (9, 7);
	connect (m_stop, SIGNAL(clicked()), client.xplayback (), SLOT(stop ()));

	m_next = new Button (this);
	m_next->move(206, 4);
	m_next->resize (8, 7);
	connect (m_next, SIGNAL(clicked()), client.xplayback (), SLOT(next ()));

	m_eject = new Button (this);
	m_eject->move(216, 4);
	m_eject->resize (9, 7);
	connect (m_eject, SIGNAL(clicked()), this, SLOT(fileOpen()));

	connect (&client, SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(setStatus(Xmms::Playback::Status)));
	connect (client.cache (), SIGNAL (playtime (uint32_t)),
	         this, SLOT ( setPlaytime(uint32_t)));
	connect (&client, SIGNAL(currentSong (const Xmms::PropDict &)), 
			 this, SLOT(setMediainfo (const Xmms::PropDict &)));
}

void
ShadedDisplay::setMediainfo (const Xmms::PropDict &info)
{
	QString n;
	if (info.contains ("artist") && info.contains ("album") &&
	    info.contains ("title")) {
		n = QString::fromUtf8 (info.get<std::string> ("artist").c_str ())
		    + " - " +
		    QString::fromUtf8 (info.get<std::string> ("album").c_str ())
		    + " - " +
		    QString::fromUtf8 (info.get<std::string> ("title").c_str ());
	} else {
		n = QString::fromUtf8 (info.get<std::string> ("url").c_str ());
	}
	m_title->setText (n);
}

void
ShadedDisplay::setStatus (Xmms::Playback::Status status)
{
	if (status == Xmms::Playback::STOPPED) {
		m_number->setNumber (0, 2);
		m_number2->setNumber (0, 2);
	}
}

void
ShadedDisplay::setPlaytime (uint32_t time)
{
	uint sec, min;

	sec = (time / 1000) % 60;
	min = (time / 1000) / 60;

		m_number2->setNumber (sec, 2);
		m_number->setNumber (min, 2);
}



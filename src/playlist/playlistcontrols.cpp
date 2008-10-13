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

#include "playlistcontrols.h"
#include "pixmapbutton.h"
#include "timedisplay.h"

PlaylistControls::PlaylistControls (QWidget *parent) : QWidget (parent)
{
	setFixedSize (100, 38);

	/*
	 * Buttons
	 */
	PixmapButton *button;
	//prev button
	button = new PixmapButton (this);
	button->resize (9, 8);
	button->move (6, 23);
	connect (button, SIGNAL (clicked ()), this, SIGNAL (prev ()));
	//play button
	button = new PixmapButton (this);
	button->resize (9, 8);
	button->move (15, 23);
	connect (button, SIGNAL (clicked ()), this, SIGNAL (play ()));
	//pause button
	button = new PixmapButton (this);
	button->resize (9, 8);
	button->move (24, 23);
	connect (button, SIGNAL (clicked ()), this, SIGNAL (pause ()));
	//stop button
	button = new PixmapButton (this);
	button->resize (9, 8);
	button->move (33, 23);
	connect (button, SIGNAL (clicked ()), this, SIGNAL (stop ()));
	//next button
	button = new PixmapButton (this);
	button->resize (9, 8);
	button->move (42, 23);
	connect (button, SIGNAL (clicked ()), this, SIGNAL (next ()));
	//eject button
	button = new PixmapButton (this);
	button->resize (9, 8);
	button->move (52, 23);
	connect (button, SIGNAL (clicked ()), this, SIGNAL (eject ()));

	m_timedisplay = new SmallTimeDisplay (this);
	m_timedisplay->move (69, 23);
//	m_timedisplay->hide ();
	connect (m_timedisplay, SIGNAL (clicked ()),
	         this, SIGNAL (toggleTime ()));
	connect (this, SIGNAL (setDisplayTime (int)),
	         m_timedisplay, SLOT (setTime (int)));

	//TODO:  playtimes
}

void
PlaylistControls::setNumbers (const PixmapMap &p)
{
	m_timedisplay->setPixmaps (p);
}

void
PlaylistControls::setSelectedLength (int lenght)
{
	//TODO
}

void
PlaylistControls::setPlaylistLength (int lenght)
{
	//TODO
}

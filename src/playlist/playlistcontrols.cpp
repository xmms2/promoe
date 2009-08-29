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

#include <QPainter>

QString secondsToString (uint32_t seconds)
{
	int h, m, s;
	s = seconds % 60;
	seconds /= 60;
	m = seconds % 60;
	h = seconds / 60;

	QString ret = QString ("%1%2:%3")
		.arg (h ? QString ("%1:").arg(h) : "")
		.arg (m, h > 0 ? 2 : 1, 10, QLatin1Char('0'))
		.arg (s, 2, 10, QLatin1Char('0'));
	return ret;
}


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
	m_timedisplay->move (68, 23);
	connect (m_timedisplay, SIGNAL (clicked ()),
	         this, SIGNAL (toggleTime ()));
	connect (this, SIGNAL (setDisplayTime (int)),
	         m_timedisplay, SLOT (setTime (int)));

	m_selection_playtime = 0;
	m_playlist_playtime = 0;
	is_playlist_playtime_exact = true;
}

void
PlaylistControls::paintEvent (QPaintEvent *event)
{
	if (m_font.isEmpty ()) return;

	int x = 8, y = 10;
	QString time = QString ("%1/%2%3")
		.arg (secondsToString (m_selection_playtime))
		.arg (secondsToString (m_playlist_playtime))
		.arg (is_playlist_playtime_exact ? "" : "+");

	QPainter paint;
	paint.begin (this);
	foreach (QChar c, time) {
		paint.drawPixmap (x, y, m_font[c.toAscii()]);
		x += 5;
	}
	paint.end ();

}

void
PlaylistControls::setNumbers (const PixmapMap &p)
{
	m_timedisplay->setPixmaps (p);
}

void
PlaylistControls::setPixmapFont (const PixmapMap &p)
{
	m_font = p;
	update ();
}

void
PlaylistControls::setSelectionPlaytime (uint32_t playtime)
{
	m_selection_playtime = playtime;
	update ();
}

void
PlaylistControls::setPlaylistPlaytime (uint32_t playtime, bool isExact)
{
	m_playlist_playtime = playtime;
	is_playlist_playtime_exact = isExact;
	update ();
}

#include "playlistcontrols.moc"

/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include <xmmsclient/xmmsclient++.h>

#include "xplayback.h"
#include "xclient.h"

#include <xmmsclient/xmmsclient++/playback.h>
#include <xmmsclient/xmmsclient++/playlist.h>

XPlayback::XPlayback (XClient *client)
{
	m_client = client;
}

void
XPlayback::play ()
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->start ();
}

void
XPlayback::pause ()
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->pause ();
}

void
XPlayback::stop ()
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->stop ();
}

void
XPlayback::prev ()
{
	if (!m_client->isConnected ()) return;

	m_client->playlist ()->setNextRel (-1);
	m_client->playback ()->tickle ();
}

void
XPlayback::next ()
{
	if (!m_client->isConnected ()) return;

	m_client->playlist ()->setNextRel (1);
	m_client->playback ()->tickle ();
}

/*
 * Seeking
 */
void
XPlayback::seekMs (uint milliseconds)
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->seekMs (milliseconds);
}

void
XPlayback::seekMsRel (int milliseconds)
{
	if (!m_client->isConnected ()) return;

	m_client->playback ()->seekMsRel (milliseconds);
}

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

#include <QtDebug>

#include <QStyle>

XPlayback::XPlayback (XClient *client)
{
	m_client = client;

	m_volume = 0;
	m_balance = 0;

	connect (client, SIGNAL (gotConnection (XClient *)),
	         this, SLOT (on_connect (XClient *)));

	if (client->isConnected ()) {
		on_connect (client);
	}
}

void
XPlayback::on_connect (XClient *client)
{
	client->playback ()->getStatus ()
	        (Xmms::bind (&XPlayback::playback_status, this));
	client->playback ()->broadcastStatus ()
	        (Xmms::bind (&XPlayback::playback_status, this));

	client->playback ()->volumeGet ()
			(Xmms::bind (&XPlayback::volume_changed, this));
	client->playback ()->broadcastVolumeChanged ()
	        (Xmms::bind (&XPlayback::volume_changed, this));
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
XPlayback::toggle_pause ()
{
	if (m_status == XMMS_PLAYBACK_STATUS_PLAY) {
		pause ();
	} else if (m_status == XMMS_PLAYBACK_STATUS_PAUSE) {
		play ();
	}

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

void
XPlayback::setPos (int pos)
{
	if (!m_client->isConnected ()) return;

	m_client->playlist ()->setNext (pos) ();
	m_client->playback ()->tickle () ();
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

/*
 * Status signals
 */
bool
XPlayback::playback_status (const Xmms::Playback::Status &status)
{
	m_status = status;

	emit playbackStatusChanged (status);
	return true;
}

/*
 * Volume
 */
inline int
calcBalance (int left, int right)
{
	if (left == right)
		return 0;
	if (left == 0)
		return MAX_BALANCE;
	if (right == 0)
		return -MAX_BALANCE;

	//FIXME: This somehow works, but I'm not happy with it as
	// QStyle::sliderValueFromPosition is not intended for this
	if (left > right)
		return -QStyle::sliderValueFromPosition(0, MAX_BALANCE, right, left, true);
	else
		return QStyle::sliderValueFromPosition(0, MAX_BALANCE, left, right, true);
}

bool
XPlayback::volume_changed (const Xmms::Dict &volDict)
{
	QHash<QString, QVariant> levels = XClient::convert_dict (volDict);

	if (levels.size () == 1) {
		m_onechannel = true;
		newVolume (levels.values ().first ().toInt ());
		newBalance (0);
	} else {
		/*
		 * I might add a configure option later, to map arbitrary keys to
		 * left and right
		 */
		if (!levels.contains ("left") || !levels.contains ("right")) {
			qWarning () << "Could not get volume levels, dict contains keys: "
			            << levels.keys ();
			// disable further updates. Otherwise we would spam the console
			return false;
		}
		int left = levels["left"].toInt ();
		int right = levels["right"].toInt ();
		newVolume (qMax (right, left));
		newBalance (calcBalance (left, right));
	}


	return true;
}

void
XPlayback::newVolume (int new_volume)
{
	// only emit signal if the volume really changed
	if (new_volume == m_volume)
		return;

	m_volume = new_volume;
	emit volumeChanged (new_volume);
}

void
XPlayback::newBalance (int new_balance)
{
	// only emit signal if balance really changed
	if (new_balance == m_balance)
		return;

	m_balance = new_balance;
	emit balanceChanged (new_balance);
}


int
calcVolume (int volume, int balance)
{
	balance = qAbs (balance);
	if (balance > MAX_BALANCE) {
		qWarning () << "Error in calculating balance, value " << balance
		            << "is outside valid range";
		return 0;
	}

	if (balance == 0)
		return volume;
	if (balance == MAX_BALANCE)
		return 0;

	//FIXME: this somehow works, but I'm not happy with it as
	// QStyle::sliderPositionFromValue is not intended for this
	return QStyle::sliderPositionFromValue(0, MAX_BALANCE, balance, volume, true);
}

void
XPlayback::setVolume (int new_volume)
{
	if (!m_client->isConnected ()) return;

	// Don't echo values the server sent us back to it
	if (m_volume == new_volume)
		return;

	//TODO: some error checking
	if (m_onechannel) {
		m_client->playback ()->volumeSet ("master", new_volume);
	} else {
		int right, left;
		if (m_balance < 0) {
			left = new_volume;
			right = calcVolume (new_volume, m_balance);
		} else {
			left = calcVolume (new_volume, m_balance);
			right = new_volume;
		}
		m_client->playback ()->volumeSet ("left", left);
		m_client->playback ()->volumeSet ("right", right);
	}

	m_volume = new_volume;
}

void
XPlayback::setBalance (int new_balance)
{
	if (!m_client->isConnected ()) return;

	// Don't echo values the server sent back to the server
	if ((m_balance == new_balance) || m_onechannel)
		return;

	if (new_balance < 0) {
		if (m_balance > 0) {
			m_client->playback ()->volumeSet ("left", m_volume);
		}
		m_client->playback ()->volumeSet ("right",
		          calcVolume (m_volume, new_balance));
	} else {
		if (m_balance < 0) {
			m_client->playback ()->volumeSet ("right", m_volume);
		}
		m_client->playback ()->volumeSet ("left",
		          calcVolume (m_volume, new_balance));
	}

	m_balance = new_balance;
}

#include "xplayback.moc"

/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#include "volumehandler.h"

#include "xclient.h"

#include <QStyle>
#include <xplayback.h>

struct volumes {
	int left;
	int right;
};

// The balance calculation still need some tweaking, as modifying volume
// also changes balance (towards 0). So changing volume multiple times causes
// balance to be reset to center

static int
calcBalance (int left, int right)
{
    if (left == right)
        return 0;
    if (left == 0)
        return MAX_STEREO_BALANCE;
    if (right == 0)
        return -MAX_STEREO_BALANCE;

	double tmp = static_cast<double>(right-left) / static_cast<double>(qMax(left, right));

	tmp *= static_cast<double>(MAX_STEREO_BALANCE);
	tmp += 0.5;

	return static_cast<int>(tmp);

}


static struct volumes
calcVolumes (int volume, int balance)
{
	struct volumes vols = {volume, volume};

	if (balance == 0) {
		return vols;
	}

	if (balance >= MAX_STEREO_BALANCE) {
		vols.left = 0;
		return vols;
	}

	if (balance <= -MAX_STEREO_BALANCE) {
		vols.right = 0;
		return vols;
	}

	int dec = (static_cast<double>(volume) / static_cast<double>(MAX_STEREO_BALANCE) * static_cast<double>(qAbs(balance))) + 0.5;

	if (balance > 0) {
		vols.left -= dec;
	} else {
		vols.right -= dec;
	}

	return vols;
}


VolumeHandler::VolumeHandler(const XClient *client) : QObject(),
                                                      m_client (client)
{
	m_balance = 0;
	m_volume = 0;

	keys_left << "left";
	keys_right << "right";

	connect (m_client->xplayback (), SIGNAL(volumeChanged(VolumeMap)),
	         this, SLOT(serverVolumeChanged (VolumeMap)));
}

void VolumeHandler::serverVolumeChanged (VolumeMap volumes)
{
	if (m_volumes.size () <= 1 && volumes.size () > 1) {
		emit mono(false);
	}

	if (m_volumes.size () > 1 && volumes.size () <= 1) {
		emit mono(true);
	}

	m_volumes = volumes;
	int _balance = 0;
	int _volume = 0;

	if (volumes.isEmpty()) {
		channel_left_mono = QString ();
		channel_right = QString ();
	} else if (isMono ()) {
		channel_left_mono = m_volumes.keys ().first ();
		channel_right = QString ();
		_volume = m_volumes.value (channel_left_mono);
	} else {
		channel_left_mono = channel_right = QString ();
		QStringList keys = m_volumes.keys ();
		foreach (QString lkey,  keys) {
			if (keys_left.contains (lkey)) {
				channel_left_mono = lkey;
				break;
			}
		}
		foreach (QString rkey,  keys) {
			if (keys_right.contains (rkey)) {
				channel_right = rkey;
				break;
			}
		}
		if (channel_left_mono.isEmpty () || channel_right.isEmpty ()) {
			qWarning () << "Could not parse left and right volume from: "
			            << keys;
		} else {
			int lvalue = m_volumes.value(channel_left_mono);
			int rvalue = m_volumes.value(channel_right);
			_volume = qMax (lvalue, rvalue);
			_balance = calcBalance (lvalue, rvalue);
		}
	}

	if (_volume != m_volume) {
		m_volume = _volume;
		emit volume(_volume);
	}
	if (_balance != m_balance) {
		m_balance = _balance;
		emit balance(m_balance);
	}

}

void VolumeHandler::setVolume (QString key, int volume)
{
	if (key.isEmpty ())
		return;

	if (!m_volumes.contains (key))
		return;

	if (m_volumes.value (key) == volume)
		return;

	m_client->xplayback ()->setVolume (key, volume);
}


void VolumeHandler::setVolume (int _volume, int _balance)
{
	if (m_volumes.isEmpty ())
		return;

	if (isMono ())
		_balance = 0;

	if (_volume == m_volume && _balance == m_balance)
		return;

	const struct volumes vols = calcVolumes (_volume, _balance);

	setVolume (channel_left_mono, vols.left);
	setVolume (channel_right, vols.right);
}

// calculate volumes for left and right and set them in the server
void VolumeHandler::setVolume (int volume)
{
	setVolume (volume, m_balance);
}

// calculate volumes for left and right and set them in the server
void VolumeHandler::setBalance (int balance)
{
	setVolume (m_volume, balance);
}

#include "volumehandler.moc"

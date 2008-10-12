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

#ifndef __XPLAYBACK_H__
#define __XPLAYBACK_H__

class XClient;

#include <xmmsclient/xmmsclient++.h>
#include <QObject>

static const int MAX_BALANCE = 20;

/**
 * @class XPlayback
 * @brief Thin wrapper around Xmms::Playback providing QT Signals and Slots
 *
 */
class XPlayback : public QObject {
	Q_OBJECT

	public:
		XPlayback (XClient *);

		int getVolume () {return m_volume;}
		int getBalance () {return m_balance;}

	public slots:
		void play ();
		void pause ();
		void toggle_pause ();
		void stop ();
		void prev ();
		void next ();
		void setPos (int pos);

		void seekMs (uint milliseconds);
		void seekMsRel (int milliseconds);

		void setVolume (int new_volume);
		void setBalance (int new_balance);

		// callbacks for clientlib
		bool playback_status (const Xmms::Playback::Status &status);
		bool volume_changed (const Xmms::Dict &volDict);

		void on_connect (XClient *);

	signals:
		void playbackStatusChanged (Xmms::Playback::Status status);
		void volumeChanged (int volume);
		void balanceChanged (int balance);

	private:
		XClient *m_client;
		Xmms::Playback::Status m_status;

		void newVolume (int new_volume);
		void newBalance (int new_balance);
		int m_volume;
		int m_balance;
		bool m_onechannel;
};

#endif

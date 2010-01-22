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
#include <QMap>

typedef QMap<QString, int> VolumeMap;

/**
 * @class XPlayback
 * @brief Thin wrapper around Xmms::Playback providing QT Signals and Slots
 *
 */
class XPlayback : public QObject {
	Q_OBJECT

	public:
		XPlayback (XClient *);

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
		// Helper to directly connect sliders to this class
		void seekMs (int milliseconds) {seekMs ((uint) milliseconds);};

		void setVolume (QString key, int volume);

		// callbacks for clientlib
		bool playback_status (const Xmms::Playback::Status &status);
		bool volume_changed (const Xmms::Dict &volDict);

		void on_connect (XClient *);

	signals:
		void playbackStatusChanged (Xmms::Playback::Status status);
		void volumeChanged (VolumeMap);

	private:
		XClient *m_client;
		Xmms::Playback::Status m_status;

};

#endif

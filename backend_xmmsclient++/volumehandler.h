/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2010 XMMS2 Team
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

#ifndef __VOLUMEHANDLER_H__
#define __VOLUMEHANDLER_H__

#include <QObject>
#include <QMap>
#include <QString>
#include <QStringList>

class XClient;

typedef QMap<QString, int> VolumeMap;

static const int MAX_STEREO_BALANCE = 20;

class VolumeHandler : public QObject
{
	Q_OBJECT

	public:
		VolumeHandler (const XClient *);

		VolumeMap getVolumes () const {
			return m_volumes;
		};

		QStringList getVolumeKeys () const {
			return m_volumes.keys();
		}

		bool isMono () const {
			return (m_volumes.size() == 1);
		}

	public slots:
		void serverVolumeChanged (VolumeMap);

		void setVolume (QString, int);

		void setVolume (int volume, int balance);
		void setVolume (int);
		void setBalance (int);

	signals:
		void volume (int volume);
		void balance (int balance);
		void mono (bool);

	protected:
		const XClient *m_client;

		int m_volume;
		int m_balance;
		QString channel_left_mono;
		QString channel_right;
		VolumeMap m_volumes;

		QStringList keys_left;
		QStringList keys_right;
};

#endif

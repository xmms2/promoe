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

#ifndef __XMMS_HANDLER_H__
#define __XMMS_HANDLER_H__

#include <xmmsclient/xmmsclient++.h>

#include "xclient.h"
#include "xmmsqt4.h"

#include <QObject>
#include <QHash>

class PlaylistModel;

class XMMSHandler : public XClient {
	Q_OBJECT
	public:
		static XMMSHandler &getInstance ();

		XMMSHandler (QObject *parent, const std::string &name);
		~XMMSHandler () {};

		bool connect_handler (const char *ipcpath = NULL, const bool &sync = false, QWidget *parent = NULL);

		bool playback_current_id (const unsigned int &id);
		bool medialib_info (const Xmms::PropDict &propdict);
		bool medialib_entry_changed (const unsigned int &id);
		bool volume_changed (const Xmms::Dict &levels);

		void requestMediainfo (uint id);
		void playlistAddURL (const QString& url);

		/*
		void medialib_select (XMMSResultDictList *res);


		*/
		/*
		uint medialibQuery (QString);
		void medialibQueryAdd (QString q) { delete m_xmmsc->medialib_add_to_playlist (q.toUtf8 ()); }
		*/

		void volumeGet ();

		//const XMMSClient *getXMMS () { return m_xmmsc; }

		void updateSettings () { emit settingsSaved (); }

		Xmms::Client *getClient ();

		PlaylistModel *getPlaylistModel () {return m_playlist_model; }

	public slots:
		void volumeSet (uint volume);

	signals:
		void settingsSaved ();
		void mediainfoChanged (uint, const Xmms::PropDict &);
		void currentSong (const Xmms::PropDict &);
		void playlistChanged (const Xmms::Dict &);
		/*
		void medialibResponse (uint, const QList<QHash<QString, QString> > &);
		*/
		void getVolume (uint);

	private:
		void DictToQHash (const std::string &key,
		                  const Xmms::Dict::Variant &value,
		                  QHash<QString, QString> &hash);
//	void PropDictToQHash (const std::string &key,
//		                      const Xmms::Dict::Variant &value,
//		                      const std::string &source,
//		                      QHash<QString, QString> &hash);

		bool volume_get (const Xmms::Dict &levels);
		bool volume_error (const std::string &error);

		XmmsQT4 *m_qt4;
		PlaylistModel *m_playlist_model;
		unsigned int m_currentid;
		bool m_masterchan;
};

#endif


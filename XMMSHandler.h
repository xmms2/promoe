#ifndef __XMMS_HANDLER_H__
#define __XMMS_HANDLER_H__

#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"

#include <QObject>
#include <QHash>
#include <QTimer>

class XMMSHandler : public QObject {
	Q_OBJECT
	public:
		static XMMSHandler &getInstance ();

		bool connect (const char *path);

		bool playback_playtime (const unsigned int &time);
		bool playlist_list (const Xmms::List< unsigned int > &playlist);
		bool playback_current_id (const unsigned int &id);
		bool medialib_info (const Xmms::PropDict &propdict);
		bool medialib_entry_changed (const unsigned int &id);
		bool playback_status (const Xmms::Playback::Status &status);
		bool volume_changed (const Xmms::Dict &levels);
		bool playlist_changed (const Xmms::Dict &list);

		void requestMediainfo (uint id);
		void requestPlaylistList ();
		void requestTrackChange (int pos);
		void playlistAddURL (const QString& url);
		void playlistRemove (uint pos);
		void playlistMove (uint pos, uint newpos);

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

	public slots:
		void setPlaytime (uint pos);
		void restartPlaytime ();

		void playlistClear ();
		void play ();
		void stop ();
		void pause ();
		void next ();
		void prev ();
		void volumeSet (uint volume);

	signals:
		void settingsSaved ();
		void playbackStatusChanged (Xmms::Playback::Status status);
		void playtimeChanged (uint time);
		void mediainfoChanged (uint, const Xmms::PropDict &);
		void currentSong (const Xmms::PropDict &);
		void playlistList (const Xmms::List< unsigned int > &);
		void currentID (uint);
		void playlistChanged (const Xmms::Dict &);
		/*
		void medialibResponse (uint, const QList<QHash<QString, QString> > &);
		*/
		void getVolume (uint);

	private:
		QTimer m_playtime_timer;

		XMMSHandler ();
		~XMMSHandler () {};
		void DictToQHash (const std::string &key,
		                  const Xmms::Dict::Variant &value,
		                  QHash<QString, QString> &hash);
		void PropDictToQHash (const std::string &key,
		                      const Xmms::Dict::Variant &value,
		                      const std::string &source,
		                      QHash<QString, QString> &hash);

		bool volume_get (const Xmms::Dict &levels);
		bool volume_error (const std::string &error);

		Xmms::Client m_client;
		XmmsQT4 *m_qt4;
		unsigned int m_currentid;
		bool m_masterchan;
};

#endif


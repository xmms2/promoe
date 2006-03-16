#ifndef __XMMS_HANDLER_H__
#define __XMMS_HANDLER_H__

#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"

#include <QObject>
#include <QHash>

class XMMSHandler : public QObject, public sigc::trackable {
	Q_OBJECT
	public:
		static XMMSHandler *getInstance (void);
		~XMMSHandler ();

		bool connect (const char *path);

		void playback_playtime (XMMSResultValue<uint> *res);
		void playback_current_id (XMMSResultValue<uint> *res);
		void medialib_info (XMMSResultDict *res);
		void playlist_changed (XMMSResultDict *res);
		void playback_status (XMMSResultValue<uint> *res);
		void playlist_list (XMMSResultValueList<uint> *res);
		void medialib_entry_changed (XMMSResultValue<uint> *res);
		void medialib_select (XMMSResultDictList *res);
		void volume_changed (XMMSResult *res);
		void volume_get (XMMSResultDict *res);

		void requestMediainfo (uint id);
		void requestPlaylistList (void);
		void requestTrackChange (int pos);

		void playlistAddURL (QString);
		void playlistRemove (uint pos) { delete m_xmmsc->playlist_remove (pos); }
		void playlistMove (uint pos, uint newpos) { delete m_xmmsc->playlist_move (pos, newpos); }
		uint medialibQuery (QString);
		void medialibQueryAdd (QString q) { delete m_xmmsc->medialib_add_to_playlist (q.toUtf8 ()); }
		void volumeGet (void);
		void volumeSet (uint volume);


		const XMMSClient *getXMMS () { return m_xmmsc; }

		void updateSettings (void) { emit settingsSaved (); }

	public slots:
		void setPlaytime (uint pos);

		void playlistClear () { delete m_xmmsc->playlist_clear (); }
		void play () { delete m_xmmsc->playback_start (); }
		void stop () { delete m_xmmsc->playback_stop (); }
		void pause () { delete m_xmmsc->playback_pause (); }
		void next () { 
			delete m_xmmsc->playlist_set_next_rel (1); 
			delete m_xmmsc->playback_tickle (); 
		}
		void prev () { 
			delete m_xmmsc->playlist_set_next_rel (-1); 
			delete m_xmmsc->playback_tickle (); 
		}

	signals:
		void settingsSaved (void);
		void playbackStatusChanged (uint status);
		void playtimeChanged (uint time);
		void mediainfoChanged (uint, const QHash<QString, QString> &);
		void currentSong (const QHash<QString, QString> &);
		void playlistList (const QList<uint> &);
		void currentID (uint);
		void playlistChanged (const QHash<QString, QString> &);
		void medialibResponse (uint, const QList<QHash<QString, QString> > &);
		void getVolume (uint);

	private:
		XMMSHandler (void);
		QHash<QString, QString> PropDictToQHash (XMMSResultDict *res);
		QHash<QString, QString> DictToQHash (XMMSResultDict *res);

		XmmsQT4 *m_qt4;
		XMMSClient *m_xmmsc;
		static XMMSHandler *singleton;
		int m_currentid;
		bool m_masterchan;
};

#endif


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
		XMMSHandler (void);
		~XMMSHandler ();
		void playback_playtime (XMMSResultValue<uint> *res);
		void playback_current_id (XMMSResultValue<uint> *res);
		void medialib_info (XMMSResultDict *res);
		void playlist_changed (XMMSResultDict *res);
		void playback_status (XMMSResultValue<uint> *res);
		void playlist_list (XMMSResultValueList<uint> *res);
		void medialib_entry_changed (XMMSResultValue<uint> *res);
		void medialib_select (XMMSResultDictList *res);

		void requestMediainfo (uint id);
		void requestPlaylistList (void);
		void requestTrackChange (int pos);

		void playlistAddURL (QString);
		void playlistClear (void);
		void playlistRemove (uint pos) { delete m_xmmsc->playlist_remove (pos); }
		void playlistMove (uint pos, uint newpos) { delete m_xmmsc->playlist_move (pos, newpos); }
		void medialibQuery (QString);
		
		const XMMSClient *getXMMS () { return m_xmmsc; }

	public slots:
		void setPlaytime ();
		void fileOpen (void);

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
		void playbackStatusChanged (uint status);
		void playtimeChanged (uint time);
		void mediainfoChanged (uint, QHash<QString, QString>);
		void currentSong (QHash<QString, QString>);
		void playlistList (QList<uint>);
		void currentID (uint);
		void playlistChanged (QHash<QString, QString>);
		void medialibResponse (QList<QHash<QString, QString> >);

	private:
		XmmsQT4 *m_qt4;
		XMMSClient *m_xmmsc;
		static XMMSHandler *singleton;
		uint m_currentid;
		QHash<QString, QString> PropDictToQHash (XMMSResultDict *res);
		QHash<QString, QString> DictToQHash (XMMSResultDict *res);
};

#endif


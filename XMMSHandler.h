#ifndef __XMMS_HANDLER_H__
#define __XMMS_HANDLER_H__

#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"

#include <QObject>

class XMMSHandler : public QObject, public sigc::trackable {
	Q_OBJECT
	public:
		static XMMSHandler *getInstance (void);
		XMMSHandler (void);
		~XMMSHandler ();
		void playback_playtime (XMMSResultValue<uint> *res);
		void playback_current_id (XMMSResultValue<uint> *res);
		void medialib_info (XMMSResultDict *res);
		void playback_status (XMMSResultValue<uint> *res);
		void playlist_list (XMMSResultValueList<uint> *res);

		const XMMSClient *getXMMS () { return m_xmmsc; }

	public slots:
		void setPlaytime ();

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
		void mediainfoChanged (QString str, int bitrate, int samplerate,
		                       int channels, int duration);

	private:
		XmmsQT4 *m_qt4;
		XMMSClient *m_xmmsc;
		static XMMSHandler *singleton;
};

#endif


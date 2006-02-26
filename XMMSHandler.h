#ifndef __XMMS_HANDLER_H__
#define __XMMS_HANDLER_H__

#include <xmmsclient/xmmsclient++.h>

class XMMSHandler;

#include "MainWindow.h"
#include "XmmsQT4.h"

#include <QObject>

class XMMSHandler : public QObject, public sigc::trackable {
	Q_OBJECT
	public:
		XMMSHandler (MainWindow *mw);
		~XMMSHandler ();
		void playback_playtime (XMMSResultValueUint *res);
		void playback_current_id (XMMSResultValueUint *res);
		void medialib_info (XMMSResultDict *res);
		void playback_status (XMMSResultValueUint *res);
		void playlist_list (XMMSResultValueListUint *res);

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

	private:
		MainWindow *m_mw;
		XmmsQT4 *m_qt4;
		XMMSClient *m_xmmsc;

};

#endif


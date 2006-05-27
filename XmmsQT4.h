#ifndef __XMMSQT4_H__
#define __XMMSQT4_H__

#include <xmmsclient/xmmsclient++/mainloop.h>

#include <QApplication>
#include <QObject>
#include <QSocketNotifier>

class XmmsQT4 : public QObject, public Xmms::MainloopInterface
{
	Q_OBJECT
	public:
		XmmsQT4(xmmsc_connection_t *xmmsc);
		~XmmsQT4();

		void run ();

		void ToggleWrite(bool toggle);
		xmmsc_connection_t *GetXmmsConnection();

	public slots:
		void OnRead ();
		void OnWrite ();

	private:
		int m_fd;
		QSocketNotifier *m_rsock;
		QSocketNotifier *m_wsock;
		xmmsc_connection_t *m_xmmsc;
};

#endif

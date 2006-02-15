#ifndef __XMMSQT4_H__
#define __XMMSQT4_H__

#include <iostream>

#include <QApplication>
#include <QObject>
#include <QSocketNotifier>

#include <xmmsclient/xmmsclient.h>


class XmmsQT4 : public QObject
{
	Q_OBJECT
	public:
		XmmsQT4(xmmsc_connection_t *xmmsc = 0, QObject *parent = 0);
		~XmmsQT4();

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

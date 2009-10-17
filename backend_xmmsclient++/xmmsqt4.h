/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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

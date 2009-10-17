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


#include <xmmsclient/xmmsclient++/mainloop.h>
#include <xmmsclient/xmmsclient.h>
#include "xmmsqt4.h"

#include <QApplication>
#include <QObject>
#include <QSocketNotifier>


static void CheckWrite (int i, void *userdata);

XmmsQT4::XmmsQT4 (xmmsc_connection_t *xmmsc) :
	QObject (), Xmms::MainloopInterface (xmmsc),
	m_fd (0), m_rsock (0), m_wsock (0), m_xmmsc (xmmsc)
{
	m_fd = xmmsc_io_fd_get (xmmsc);
	xmmsc_io_need_out_callback_set (xmmsc, CheckWrite, this);

	m_rsock = new QSocketNotifier (m_fd, QSocketNotifier::Read, this);
	connect (m_rsock, SIGNAL (activated (int)), SLOT (OnRead ()));
	m_rsock->setEnabled (true);

	m_wsock = new QSocketNotifier (m_fd, QSocketNotifier::Write, this);
	connect (m_wsock, SIGNAL (activated (int)), SLOT (OnWrite ()));
	m_wsock->setEnabled (false);
	running_ = true;
}


XmmsQT4::~XmmsQT4 ()
{
	delete m_rsock;
	delete m_wsock;
}

void XmmsQT4::run ()
{
}

xmmsc_connection_t *XmmsQT4::GetXmmsConnection ()
{
	return m_xmmsc;
}


void XmmsQT4::OnRead ()
{
	if (!xmmsc_io_in_handle (m_xmmsc)) {
		return; /* exception? */
	}
}


void XmmsQT4::OnWrite ()
{
	if (!xmmsc_io_out_handle (m_xmmsc)) {
		return; /* exception? */
	}
}

void XmmsQT4::ToggleWrite (bool toggle)
{
	m_wsock->setEnabled (toggle);
}

static void CheckWrite (int i, void *userdata)
{
	XmmsQT4 *obj = static_cast< XmmsQT4* > (userdata);

	if (xmmsc_io_want_out (obj->GetXmmsConnection ())) {
		obj->ToggleWrite (true);
	} else {
		obj->ToggleWrite (false);
	}
}

#include "xmmsqt4.moc"

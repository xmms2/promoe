#include "XmmsQT4.h"

using namespace std;

static void CheckWrite (int i, void *userdata);

XmmsQT4::XmmsQT4 (xmmsc_connection_t *xmmsc, QObject *parent) : QObject (parent)
{
	m_xmmsc = xmmsc;

	m_fd = xmmsc_io_fd_get (xmmsc);
	xmmsc_io_need_out_callback_set (xmmsc, CheckWrite, this);

	m_rsock = new QSocketNotifier (m_fd, QSocketNotifier::Read, this);
	connect (m_rsock, SIGNAL (activated (int)), SLOT (OnRead ()));
	m_rsock->setEnabled (true);

	m_wsock = new QSocketNotifier (m_fd, QSocketNotifier::Write, this);
	connect (m_wsock, SIGNAL (activated (int)), SLOT (OnWrite ()));
	m_wsock->setEnabled (false);
}


XmmsQT4::~XmmsQT4 ()
{
	delete m_rsock;
	delete m_wsock;
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
	XmmsQT4 *obj = (XmmsQT4 *) userdata;
	xmmsc_connection_t *xmmsc = obj->GetXmmsConnection();

	if (xmmsc_io_want_out (xmmsc)) {
		obj->ToggleWrite (true);
	} else {
		obj->ToggleWrite (false);
	}
}


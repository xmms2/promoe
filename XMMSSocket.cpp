#include <xmmsclient/xmmsclient++.h>

#include <QErrorMessage>
#include "XMMSSocket.h"

XMMSSocket::XMMSSocket (const char *name) :
	m_client (name)
{
}

bool
XMMSSocket::connect (const char *path)
{
	try {
		m_client.connect (path ? path : "");
	}
	catch (Xmms::connection_error& e) {
		QErrorMessage *err = new QErrorMessage ();
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		delete err;
		return false;
	}

	m_client.setMainloop (new XmmsQT4 (m_client.getConnection ()));

	return true;
}


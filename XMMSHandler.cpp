#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"
#include "MainDisplay.h"
#include "XMMSHandler.h"

#include <QErrorMessage>

XMMSHandler::XMMSHandler (MainWindow *mw) : sigc::trackable ()
{
	m_mw = mw;

	m_xmmsc = new XMMSClient ("promoe");

	if (!m_xmmsc->connect (NULL)) {
		QErrorMessage *err = new QErrorMessage ();
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		exit (-1);
	}
	m_qt4 = new XmmsQT4 (m_xmmsc->getConn (), qApp);

	XMMSResult *r = m_xmmsc->signal_playback_playtime ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_playtime));

	r = m_xmmsc->broadcast_playback_current_id ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_current_id));
}

void 
XMMSHandler::playback_playtime (XMMSResult *res)
{
	uint i, sec, min;
	res->getValue (&i);

	sec = (i / 1000) % 60;
	min = (i / 1000) / 60;
	m_mw->getMD ()->m_number->setNumber (min / 10, min % 10);
	m_mw->getMD ()->m_number2->setNumber (sec / 10, sec % 10);

	res->restart ();
}

void 
XMMSHandler::playback_current_id (XMMSResult *res)
{
	uint i;
	res->getValue (&i);

	qDebug ("current id = %d", i);

	XMMSResult *r = m_xmmsc->medialib_get_info (i);
	r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_info));
}

void 
XMMSHandler::medialib_info (XMMSResult *res)
{
	char str[4096];
	int b;

	/* Make this NICER! */
	res->entryFormat (str, 4096, "${artist} - ${album} - ${title}");
	qDebug ("%s", str);
	m_mw->getMD ()->m_text->setText (QString::fromUtf8 (str));

	if (res->getDictValue ("bitrate", &b)) {
		m_mw->getMD ()->m_kbps->setNumber (b/1000, 3);
	}

	if (res->getDictValue ("samplerate", &b)) {
		m_mw->getMD ()->m_khz->setNumber (b/1000, 2);
	}

	delete res;
}

XMMSHandler::~XMMSHandler ()
{
}


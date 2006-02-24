#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"
#include "MainDisplay.h"
#include "XMMSHandler.h"

#include <QErrorMessage>

XMMSHandler::XMMSHandler (MainWindow *mw) : sigc::trackable ()
{
	m_mw = mw;

	m_xmmsc = new XMMSClient ("promoe");

	if (!m_xmmsc->connect (getenv ("XMMS_PATH"))) {
		QErrorMessage *err = new QErrorMessage ();
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		exit (-1);
	}
	m_qt4 = new XmmsQT4 (m_xmmsc->getConn (), qApp);

	XMMSResultValueUint *r = m_xmmsc->signal_playback_playtime ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_playtime));

	r = m_xmmsc->playback_current_id ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_current_id));

	r = m_xmmsc->broadcast_playback_current_id ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_current_id));

	r = m_xmmsc->broadcast_playback_status ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_status));
}

void
XMMSHandler::playback_status (XMMSResultValueUint *res)
{
	uint i;
	res->getValue (&i);
	m_mw->getMD ()->m_playstatus->setStatus (i);

	if (i == XMMS_PLAYBACK_STATUS_STOP) {
		m_mw->getSD ()->m_number->setNumber (0, 2);
		m_mw->getSD ()->m_number2->setNumber (0, 2);
		m_mw->getMD ()->m_number->setNumber (0, 0);
		m_mw->getMD ()->m_number2->setNumber (0, 0);
		m_mw->getMD ()->m_slider->setPos (0);
	}

}

void 
XMMSHandler::playback_playtime (XMMSResultValueUint *res)
{
	uint i, sec, min;
        res->getValue (&i);

	sec = (i / 1000) % 60;
	min = (i / 1000) / 60;

	if (m_mw->getShaded ()) {
		m_mw->getSD ()->m_number->setNumber (min, 2);
		m_mw->getSD ()->m_number2->setNumber (sec, 2);
	} else {
		m_mw->getMD ()->m_number->setNumber (min / 10, min % 10);
		m_mw->getMD ()->m_number2->setNumber (sec / 10, sec % 10);

		/* update slider */
		m_mw->getMD ()->m_slider->setPos (i);
	}

	res->restart ();
}

void 
XMMSHandler::playback_current_id (XMMSResultValueUint *res)
{
	uint i;
	res->getValue (&i);

	if (i > 0) {
		XMMSResultDict *r = m_xmmsc->medialib_get_info (i);
		r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_info));
	}

	if (res->getClass() == XMMSC_RESULT_CLASS_DEFAULT) {
		delete res;
	}
}

void
XMMSHandler::setPlaytime (void)
{
	uint pos = m_mw->getMD ()->m_slider->getPos();
	qDebug ("pos = %d", pos);
	delete m_xmmsc->playback_seek_ms (pos);

}

void 
XMMSHandler::medialib_info (XMMSResultDict *res)
{
	char str[4096];
	int b;

	/* Make this NICER! */
	res->entryFormat (str, 4096, "${artist} - ${album} - ${title}");
	m_mw->getMD ()->m_text->setText (QString::fromUtf8 (str));
	m_mw->getSD ()->m_title->setText (QString::fromUtf8 (str));

	if (res->getValue ("bitrate", &b)) {
		m_mw->getMD ()->m_kbps->setNumber (b/1000, 3);
	}

	if (res->getValue ("samplerate", &b)) {
		m_mw->getMD ()->m_khz->setNumber (b/1000, 2);
	}

	if (res->getValue ("channels:out", &b)) {
		if (b == 1) {
			m_mw->getMD ()->m_stereo->setStereoMono (0, 1);
		} else {
			m_mw->getMD ()->m_stereo->setStereoMono (1, 0);
		}
	}

	if (res->getValue ("duration", &b)) {
		if (b > 0) {
			m_mw->getMD ()->m_slider->setMax (b);
			m_mw->getMD ()->m_slider->hideBar (false);
		}
	}

	delete res;
}

XMMSHandler::~XMMSHandler ()
{
}


#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"
#include "XMMSHandler.h"

#include <QErrorMessage>
#include <QHash>

XMMSHandler *XMMSHandler::singleton = NULL;

XMMSHandler *XMMSHandler::getInstance (void)
{
	if (!singleton) {
		singleton = new XMMSHandler ();
	}

	return singleton;
}

XMMSHandler::XMMSHandler (void) : sigc::trackable ()
{
	m_xmmsc = new XMMSClient ("promoe");

	if (!m_xmmsc->connect (getenv ("XMMS_PATH"))) {
		QErrorMessage *err = new QErrorMessage ();
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		exit (-1);
	}
	m_qt4 = new XmmsQT4 (m_xmmsc->getConn (), qApp);

	XMMSResultValueList<uint> *l = m_xmmsc->playlist_list ();
	l->connect (sigc::mem_fun (this, &XMMSHandler::playlist_list));

	XMMSResultValue<uint> *r = m_xmmsc->signal_playback_playtime ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_playtime));

	r = m_xmmsc->playback_current_id ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_current_id));

	r = m_xmmsc->broadcast_playback_current_id ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_current_id));

	r = m_xmmsc->broadcast_playback_status ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_status));
}

void
XMMSHandler::requestMediainfo (uint id)
{
	XMMSResultDict *r = m_xmmsc->medialib_get_info (id);
	r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_info));
}

void
XMMSHandler::playlist_list (XMMSResultValueList<uint> *res) 
{
	QList<uint> list;
	for (;res->listValid (); res->listNext()) {
		uint i;
		if (res->getValue (&i)) {
			list.append (i);
		}
	}
	emit playlistList (list);

	delete res;
}

void
XMMSHandler::playback_status (XMMSResultValue<uint> *res)
{
	uint status;
	res->getValue (&status);

	emit playbackStatusChanged (status);
}

void 
XMMSHandler::playback_playtime (XMMSResultValue<uint> *res)
{
	uint i;
	res->getValue (&i);

	emit playtimeChanged (i);
	
	res->restart ();
}

void 
XMMSHandler::playback_current_id (XMMSResultValue<uint> *res)
{
	uint i;
	res->getValue (&i);

	m_currentid = i;

	if (i > 0) {
		XMMSResultDict *r = m_xmmsc->medialib_get_info (i);
		r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_info));
	}

	emit currentID(i);

	if (res->getClass() == XMMSC_RESULT_CLASS_DEFAULT) {
		delete res;
	}
}

void
XMMSHandler::setPlaytime (void)
{
	/*
	uint pos = m_mw->getMD ()->m_slider->getPos();
	qDebug ("pos = %d", pos);
	delete m_xmmsc->playback_seek_ms (pos);
	*/

}

void 
XMMSHandler::medialib_info (XMMSResultDict *res)
{
	int id;
	QHash<QString, QString> h;
	std::list<const char *> l = res->getPropDictKeys ();

	std::list<const char *>::const_iterator it;
	for(it=l.begin(); it!=l.end(); ++it)
	{
		if (res->getDictValueType (*it) == XMMSC_RESULT_VALUE_TYPE_UINT32) {
			uint i;
			res->getValue (*it, &i);
			QString t;
			t.setNum (i);
			h.insert (QString::fromLatin1(*it), t);
		} else if (res->getDictValueType (*it) == XMMSC_RESULT_VALUE_TYPE_INT32) {
			int i;
			res->getValue (*it, &i);
			QString t;
			t.setNum (i);
			h.insert (QString::fromLatin1(*it), t);
		} else if (res->getDictValueType (*it) == XMMSC_RESULT_VALUE_TYPE_STRING) {
			char *c;
			res->getValue (*it, &c);
			h.insert (QString::fromLatin1(*it), QString::fromUtf8 (c));
		}
	}

	res->getValue ("id", &id);

	emit mediainfoChanged (id, h);

	if (id == m_currentid) {
		emit currentSong (h);
	}

	delete res;
}

XMMSHandler::~XMMSHandler ()
{
}


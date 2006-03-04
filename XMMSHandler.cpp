#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"
#include "XMMSHandler.h"

#include <QErrorMessage>
#include <QHash>
#include <QFile>
#include <QFileDialog>
#include <QDir>

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

	XMMSResultDict *p = m_xmmsc->broadcast_playlist_changed ();
	p->connect (sigc::mem_fun (this, &XMMSHandler::playlist_changed));

	XMMSResultValue<uint> *r = m_xmmsc->signal_playback_playtime ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_playtime));

	r = m_xmmsc->playback_current_id ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_current_id));

	r = m_xmmsc->broadcast_playback_current_id ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_current_id));

	r = m_xmmsc->playback_status ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_status));

	r = m_xmmsc->broadcast_playback_status ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playback_status));

	r = m_xmmsc->broadcast_medialib_entry_changed ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_entry_changed));
}

void
XMMSHandler::fileOpen (void)
{
	QStringList files = 
		QFileDialog::getOpenFileNames (NULL,
									   "Select files to play",
									   QDir::homePath(),
									   "Music (*.mp3 *.ogg *.flac *.wav *.mpc *.mp4)");

	if (files.count() > 0) {
		playlistClear ();
	}

	for (int i = 0; i < files.count(); i++) {
		playlistAddURL ("file://" + files.value(i));
	}

}



void
XMMSHandler::medialib_entry_changed (XMMSResultValue<uint> *res)
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
XMMSHandler::playlistAddURL (QString s)
{
	delete m_xmmsc->playlist_add (s.toAscii ().constData ());
}

void
XMMSHandler::playlistClear (void)
{
	delete m_xmmsc->playlist_clear ();
}

void
XMMSHandler::requestMediainfo (uint id)
{
	XMMSResultDict *r = m_xmmsc->medialib_get_info (id);
	r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_info));
}

void
XMMSHandler::requestPlaylistList (void)
{
	XMMSResultValueList<uint> *r = m_xmmsc->playlist_list ();
	r->connect (sigc::mem_fun (this, &XMMSHandler::playlist_list));
}

void
XMMSHandler::requestTrackChange (int pos)
{
	XMMSResult *r = m_xmmsc->playlist_set_next (pos);
	delete r;
	XMMSResult *r2 = m_xmmsc->playback_tickle ();
	delete r2;
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

	if (res->getClass() == XMMSC_RESULT_CLASS_DEFAULT) {
		delete res;
	}
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

QHash<QString, QString>
XMMSHandler::DictToQHash (XMMSResultDict *res)
{
	QHash<QString, QString> h;
	std::list<const char *> l = res->getDictKeys ();

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

	return h;
}

QHash<QString, QString>
XMMSHandler::PropDictToQHash (XMMSResultDict *res)
{
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

	return h;
}

void
XMMSHandler::medialibQuery (QString q)
{
	XMMSResultDictList *r = m_xmmsc->medialib_select (q.toUtf8 ());
	r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_select));
}

void
XMMSHandler::medialib_select (XMMSResultDictList *res)
{
	QList<QHash<QString, QString> > l;

	for (;res->listValid (); res->listNext()) {
		QHash<QString, QString> h(DictToQHash (static_cast<XMMSResultDict *>(res)));
		l.append (h);
	}

	emit medialibResponse (l);
}

void
XMMSHandler::playlist_changed (XMMSResultDict *res)
{
	QHash<QString, QString> h(DictToQHash (res));
	emit playlistChanged (h);
}

void 
XMMSHandler::medialib_info (XMMSResultDict *res)
{
	int id;
	QHash<QString, QString> h(PropDictToQHash (res));
	
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


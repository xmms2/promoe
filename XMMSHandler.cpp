#include <xmmsclient/xmmsclient++.h>

#include "XmmsQT4.h"
#include "XMMSHandler.h"
#include <iostream>

#include <cstdlib>
#include <string>

#include <QErrorMessage>
#include <QHash>
#include <QFile>
#include <QFileDialog>
#include <QDir>

static bool log ( /*const std::string& text = ""*/ )
{
	return false;
}

static bool logUint ( const unsigned int & /*, const std::string& text = "" */ )
{
	return false;
}

XMMSHandler &XMMSHandler::getInstance ()
{
	static XMMSHandler singleton;
	return singleton;
}

XMMSHandler::XMMSHandler () : QObject (), m_client ("promoe")
{
	connect (std::getenv ( "XMMS_PATH" ));
}

bool
XMMSHandler::connect (const char *path)
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
	m_qt4 = new XmmsQT4 (m_client.getConnection (), qApp);

	using Xmms::bind;
	m_client.playlist.list (bind (&XMMSHandler::playlist_list, this));
	m_client.playlist.broadcastChanged (
		bind (&XMMSHandler::playlist_changed, this));

	m_client.medialib.broadcastEntryChanged (
		bind (&XMMSHandler::medialib_entry_changed, this));

	m_client.playback.currentID (
		bind (&XMMSHandler::playback_current_id, this));
	m_client.playback.broadcastCurrentID (
		bind (&XMMSHandler::playback_current_id, this));

	m_client.playback.getStatus (bind (&XMMSHandler::playback_status, this));
	m_client.playback.broadcastStatus (
		bind (&XMMSHandler::playback_status, this));

	m_client.playback.broadcastVolumeChanged (
		bind (&XMMSHandler::volume_changed, this));

	QObject::connect (&m_playtime_timer, SIGNAL (timeout ()),
	                  this, SLOT (restartPlaytime ()));
	m_playtime_timer.start(0);

	return true;
}

void
XMMSHandler::restartPlaytime ()
{
	m_client.playback.getPlaytime (Xmms::bind (&XMMSHandler::playback_playtime,
	                                           this));
}

bool
XMMSHandler::medialib_entry_changed (const unsigned int &id)
{
	if (id > 0) {
		requestMediainfo (id);
	}
	return true;
}

void
XMMSHandler::playlistAddURL (const QString &s)
{
	m_client.playlist.addUrl (s.toAscii ().constData (), &log);
}
void
XMMSHandler::playlistRemove (uint pos)
{ 
	m_client.playlist.remove (pos, &log);
}

void
XMMSHandler::playlistMove (uint pos, uint newpos)
{
	m_client.playlist.move (pos, newpos, &log);
}

void
XMMSHandler::requestMediainfo (uint id)
{
	m_client.medialib.getInfo (id,
	                           Xmms::bind (&XMMSHandler::medialib_info, this));
}

void
XMMSHandler::requestPlaylistList ()
{
	m_client.playlist.list (Xmms::bind (&XMMSHandler::playlist_list, this));
}

void
XMMSHandler::requestTrackChange (int pos)
{
	m_client.playlist.setNext (pos, &logUint);
	m_client.playback.tickle (&log);
}

bool
XMMSHandler::playlist_list (const Xmms::List< unsigned int > &playlist)
{
	QList<uint> list;
	for (;playlist.isValid (); ++playlist) {
		list.append (*playlist);
	}
	emit playlistList (list);
	return false;
}

bool
XMMSHandler::playback_status (const Xmms::Playback::Status &status)
{
	emit playbackStatusChanged (status);
	return true;
}

bool 
XMMSHandler::playback_playtime (const unsigned int &time)
{
	emit playtimeChanged (time);
	m_playtime_timer.start (500);
	return false;
}

bool 
XMMSHandler::playback_current_id (const unsigned int &id)
{
	m_currentid = id;

	if (id > 0) {
		requestMediainfo (id);
	}

	emit currentID(id);
	return true;
}

void
XMMSHandler::setPlaytime (uint pos)
{
	m_client.playback.seekMs (pos, &log);
}

void
XMMSHandler::DictToQHash (const std::string &key,
                          const Xmms::Dict::Variant &value,
                          QHash<QString, QString> &hash)
{
	if (value.type () == typeid (int)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< int > (value)));
	} else if (value.type () == typeid (unsigned int)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< unsigned int > (value)));
	} else if (value.type () == typeid (unsigned int)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::fromUtf8 (boost::get< std::string > (value).c_str ()));
	}
}

void
XMMSHandler::PropDictToQHash (const std::string &key,
                              const Xmms::Dict::Variant &value,
                              const std::string &source,
                              QHash<QString, QString> &hash)
{
	if (value.type () == typeid (int)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< int > (value)));
	} else if (value.type () == typeid (unsigned int)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< unsigned int > (value)));
	} else {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::fromUtf8 (boost::get< std::string > (value).c_str()));
	}
}

/*
uint
XMMSHandler::medialibQuery (QString q)
{
	XMMSResultDictList *r = m_xmmsc->medialib_select (q.toUtf8 ());
	r->connect (sigc::mem_fun (this, &XMMSHandler::medialib_select));
	m_mlibqs->append (r);
}

void
XMMSHandler::medialib_select (XMMSResultDictList *res)
{
	QList<QHash<QString, QString> > l;

	for (;res->listValid (); res->listNext()) {
		QHash<QString, QString> h(DictToQHash (static_cast<XMMSResultDict *>(res)));
		l.append (h);
	}

	emit medialibResponse (res->getCID (), l);
}
*/

bool
XMMSHandler::playlist_changed (const Xmms::Dict &list)
{
	QHash<QString, QString> hash;
#ifdef foreach
#undef foreach
#endif
	list.foreach (boost::bind (&XMMSHandler::DictToQHash, this,
	                           _1, _2, boost::ref (hash)));
	emit playlistChanged (hash);
	return true;
}

bool 
XMMSHandler::medialib_info (const Xmms::PropDict &propdict)
{

	QHash<QString, QString> hash;	
#ifdef foreach
#undef foreach
#endif
	propdict.foreach (boost::bind (&XMMSHandler::PropDictToQHash, this,
	                               _1, _2, _3, boost::ref (hash)));
	unsigned int id = propdict.get<int>("id");
	emit mediainfoChanged (id, hash);

	if (id == m_currentid) {
		emit currentSong (hash);
	}
	return false;
}

bool
XMMSHandler::volume_error (const std::string &error)
{
	qWarning ("couldn't get volume levels!");
	return false;
}

void
XMMSHandler::volumeGet ()
{
	m_client.playback.volumeGet (Xmms::bind (&XMMSHandler::volume_get, this),
	                             Xmms::bind (&XMMSHandler::volume_error, this));
}

void
XMMSHandler::volumeSet (uint volume)
{
	if(m_masterchan)
	{
		m_client.playback.volumeSet ("master", volume, &log);
	}
	else
	{
		m_client.playback.volumeSet ("left", volume, &log);
		m_client.playback.volumeSet ("right", volume, &log);
	}
}

bool
XMMSHandler::volume_changed (const Xmms::Dict &levels)
{
	volume_get (levels);
	return true;
}

bool
XMMSHandler::volume_get (const Xmms::Dict &levels)
{
	QHash<QString, QString> hash;
#ifdef foreach
#undef foreach
#endif
	levels.foreach (boost::bind (&XMMSHandler::DictToQHash, this,
	                             _1, _2, boost::ref (hash)));
	QList<QString> Values = hash.values();
	QListIterator<QString> vol (Values);

	uint right = atol (vol.next().toAscii());
	if(vol.hasNext())
	{
		uint left = atol (vol.next().toAscii());
	
		if(left > right)
			emit getVolume (left);
		else
			emit getVolume (right);

		m_masterchan = false;
	}
	else
	{
		emit getVolume (right);
		m_masterchan = true;
	}
	return false;

}


void XMMSHandler::playlistClear ()
{
	m_client.playlist.clear (&log);
}

void XMMSHandler::play ()
{
	m_client.playback.start (&log);
}

void XMMSHandler::stop ()
{
	m_client.playback.stop (&log);
}

void XMMSHandler::pause ()
{
	m_client.playback.pause (&log);
}

void XMMSHandler::next ()
{
	m_client.playlist.setNextRel (1, &logUint);
	m_client.playback.tickle (&log);
}

void XMMSHandler::prev ()
{
	m_client.playlist.setNextRel (-1, &logUint);
	m_client.playback.tickle (&log);
}

XMMSHandler::~XMMSHandler ()
{
}

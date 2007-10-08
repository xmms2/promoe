#include <xmmsclient/xmmsclient++.h>

#include "xmmsqt4.h"
#include "XMMSHandler.h"
#include "playlistmodel.h"

#include <cstdlib>
#include <string>
#include <iostream>

#include <QErrorMessage>
#include <QHash>
#include <QFile>
#include <QFileDialog>
#include <QDir>

static bool log ( /*const std::string& text = ""*/ )
{
	return false;
}

XMMSHandler &XMMSHandler::getInstance ()
{
	static XMMSHandler singleton(NULL, "Prome_Main");
	return singleton;
}

XMMSHandler::XMMSHandler (QObject *parent, const std::string &name) : XClient (parent, name)
{
	connect_handler();
}

bool
XMMSHandler::connect_handler (const char *ipcpath, const bool &sync, QWidget *parent)
{
/*	try {
		m_client.connect(path);
		//TODO reenable checking of path
		//m_client.connect (path ? path : "");
	}
	catch (Xmms::connection_error& e) {
		QErrorMessage *err = new QErrorMessage ();
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		delete err;
		return false;
	}

	m_client.setMainloop (new XmmsQT4 (m_client.getConnection ()));
*/
	//TODO must be moved elsewere later
	m_playlist_model = new PlaylistModel(NULL, this, "_active");

	connect(ipcpath, sync, parent);

	using Xmms::bind;
//	m_client->playlist.listEntries () (bind (&XMMSHandler::playlist_list, this));
//	m_client->playlist.broadcastChanged () (bind (&XMMSHandler::playlist_changed, this));

	m_client->medialib.broadcastEntryChanged () (bind (&XMMSHandler::medialib_entry_changed, this));

	m_client->playback.currentID () (bind (&XMMSHandler::playback_current_id, this));
	m_client->playback.broadcastCurrentID () (bind (&XMMSHandler::playback_current_id, this));

	m_client->playback.getStatus () (bind (&XMMSHandler::playback_status, this));
	m_client->playback.broadcastStatus () (bind (&XMMSHandler::playback_status, this));

	m_client->playback.broadcastVolumeChanged () (bind (&XMMSHandler::volume_changed, this));

	return true;
}


Xmms::Client *
XMMSHandler::getClient ()
{
	return m_client;
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
	m_client->playlist.addUrl (s.toAscii ().constData ()) ();
}
void
XMMSHandler::playlistRemove (uint pos)
{ 
	m_client->playlist.removeEntry (pos) ();
}

void
XMMSHandler::playlistMove (uint pos, uint newpos)
{
	m_client->playlist.moveEntry (pos, newpos) ();
}

void
XMMSHandler::requestMediainfo (uint id)
{
	m_client->medialib.getInfo (id) (Xmms::bind (&XMMSHandler::medialib_info, this));
}
/*
void
XMMSHandler::requestPlaylistList ()
{
//	m_client->playlist.listEntries () (Xmms::bind (&XMMSHandler::playlist_list, this));
}
*/
void
XMMSHandler::requestTrackChange (int pos)
{
	m_client->playlist.setNext (pos) ();
	m_client->playback.tickle () ();
}

bool
XMMSHandler::playlist_list (const Xmms::List< unsigned int > &playlist)
{
	emit playlistList (playlist);
	return false;
}

bool
XMMSHandler::playback_status (const Xmms::Playback::Status &status)
{
	emit playbackStatusChanged (status);
	return true;
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
	m_client->playback.seekMs (pos) ();
}

void
XMMSHandler::DictToQHash (const std::string &key,
                          const Xmms::Dict::Variant &value,
                          QHash<QString, QString> &hash)
{
	if (value.type () == typeid (int32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< int32_t > (value)));
	} else if (value.type () == typeid (uint32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< uint32_t > (value)));
	} else if (value.type () == typeid (std::string)) {
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
	if (value.type () == typeid (int32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< int32_t > (value)));
	} else if (value.type () == typeid (uint32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QString::number (boost::get< uint32_t > (value)));
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
	emit playlistChanged (list);
	return true;
}

bool 
XMMSHandler::medialib_info (const Xmms::PropDict &propdict)
{
	unsigned int id = propdict.get<int32_t>("id");
	emit mediainfoChanged (id, propdict);

	if (id == m_currentid) {
		emit currentSong (propdict);
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
	m_client->playback.volumeGet () (Xmms::bind (&XMMSHandler::volume_get, this),
	                             Xmms::bind (&XMMSHandler::volume_error, this));
}

void
XMMSHandler::volumeSet (uint volume)
{
	if(m_masterchan)
	{
		m_client->playback.volumeSet ("master", volume) ();
	}
	else
	{
		m_client->playback.volumeSet ("left", volume) ();
		m_client->playback.volumeSet ("right", volume) ();
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
	levels.each (boost::bind (&XMMSHandler::DictToQHash, this,
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
	m_client->playlist.clear () ();
}

void XMMSHandler::play ()
{
	m_client->playback.start () ();
}

void XMMSHandler::stop ()
{
	m_client->playback.stop () ();
}

void XMMSHandler::pause ()
{
	m_client->playback.pause () ();
}

void XMMSHandler::next ()
{
	m_client->playlist.setNextRel (1) ();
	m_client->playback.tickle () ();
}

void XMMSHandler::prev ()
{
	m_client->playlist.setNextRel (-1) ();
	m_client->playback.tickle () ();
}


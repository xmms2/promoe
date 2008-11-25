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

#include <xmmsclient/xmmsclient++.h>
#include "compat.h"

#include <QObject>
#include <QHash>
#include <QVariant>
#include <QErrorMessage>
#include <QSettings>

#include "xclient.h"
#include "xclientcache.h"
#include "xplayback.h"
#include "xcollection.h"
#include "xconfig.h"
#include "xmmsqt4.h"
#include "debug.h"
#include "playlistmodel.h"

#if !HAVE_XMMSV
#include "sourcepref.h"
#endif

#include <QtDebug>

QString
XClient::stdToQ (const std::string &str)
{
	return QString::fromUtf8 (str.c_str ());
}

std::string
XClient::qToStd (const QString &str)
{
	return std::string (str.toUtf8 ().data ());
}

/*
QDir
XClient::esperanza_dir ()
{
    QDir c (stdToQ (Xmms::getUserConfDir ()));
    if (!c.cd ("clients/Esperanza")) {
        c.mkpath ("clients/Esperanza");
        if (!c.cd ("clients/Esperanza")) {
            qDebug ("couldn't open dir");
        }
        return c;
    } else {
        return c;
    }
    return QDir ();
}
*/

XClient::XClient (QObject *parent, const std::string &name) : QObject (parent), m_sync (name + "-sync")
{
	m_client = NULL;
    m_isconnected = false;
	m_cache = new XClientCache (this);
	m_config = new XConfig (this);
	m_playback = new XPlayback (this);
	m_collection = new XCollection (this);
	m_active_playlist = new PlaylistModel(this, this, "_active");
	m_name = name;
}

void XClient::disconnect ()
{
	delete m_client;
	m_client = NULL;
	m_isconnected = false;

	emit disconnected (this);
}

void
XClient::shutdownServer ()
{
	if (!m_client)
		return;
	m_client->quit ();
	/* OnWrite is called here to make sure all pending messeages get sent
	 * to the server, even if an application is closing down.
	 * If this call is removed it can no longer be guaranted that the
	 * quit messeage is sent to the server
	 */
	XmmsQT4 *notifier = dynamic_cast<XmmsQT4 *>(&m_client->getMainLoop());
	notifier->OnWrite ();

	return;
}

bool
XClient::connect (const char *ipcpath, const bool &sync, QWidget *parent)
{
	bool tried_once = false;
try_again:

	try {
		delete m_client;
		m_client = new Xmms::Client (m_name);
		if (!ipcpath || ipcpath == QLatin1String (""))
			m_client->connect (NULL);
		else
			m_client->connect (ipcpath);
	}
	catch (Xmms::connection_error& e) {
		if (ipcpath == NULL && !tried_once) {
			QSettings s;
			if (s.value ("core/autostart", true).toBool ()) {
				if (!system ("xmms2-launcher")) {
					tried_once = true;
					goto try_again;
				}
			}
		}

		QErrorMessage *err = new QErrorMessage (parent);
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		delete err;
		return false;
	}

	m_client->setMainloop (new XmmsQT4 (m_client->getConnection ()));

    if (sync) {
	    try {
		    m_sync.connect (ipcpath);
	    }
	    catch (Xmms::connection_error &e) {
		    qWarning ("Couldn't establish sync connection!");
	    }
    }

	m_isconnected = true;
	// useing normal disconnect callback, if that causes problems,
	// an own method schould be created
    setDisconnectCallback (boost::bind (&XClient::disconnect, this));
	emit gotConnection (this);

	return true;
}

void
XClient::dictToQHash (const std::string &key,
					  const Xmms::Dict::Variant &value,
					  QHash<QString, QVariant> &hash)
{
	if (value.type () == typeid (int32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QVariant (boost::get< int32_t > (value)));
	} else if (value.type () == typeid (uint32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QVariant (boost::get< uint32_t > (value)));
	} else {
		QString val;
		val = QString::fromUtf8 (boost::get< std::string > (value).c_str ());
        hash.insert (stdToQ (key), QVariant (val));
	}
}

/**
 * convert a Xmms::Dict to a QHash<QString, QVariant>
**/
QHash<QString, QVariant>
XClient::convert_dict (const Xmms::Dict &dict)
{
	QHash<QString, QVariant> hash;
	dict.each (boost::bind (&XClient::dictToQHash,
							_1, _2, boost::ref (hash)));

	return hash;
}


void
XClient::propDictToQHash (const std::string &key,
						  const Xmms::Dict::Variant &value,
						  const std::string &source,
#ifdef SOURCEPREF_HACK
                          const QList<QRegExp> &prio_list,
                          QHash<QString, int> &curr_prio,
#endif
						  QHash<QString, QVariant> &hash)
{
#ifdef SOURCEPREF_HACK
	// braces because of tmp_prio definition
	{
		int tmp_prio = getPriority (QString::fromStdString (source), prio_list);
		QString tmp_key = QString::fromStdString (key);
		// Don't add a new value if the priority of it isn't better than the
		// priority already present in hash. If there is no "*" source
		// preference, this also get's rid of values we don't want at all
		if (tmp_prio >= curr_prio.value (tmp_key, prio_list.size ()))
			return;

		// Set the priority of the current source-key combination for the key, so
		// that we do not overwrite our value with a worse source for this key.
		// (higher priority values are worse)
		curr_prio[tmp_key] = tmp_prio;
	}
#endif
	if (value.type () == typeid (int32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QVariant (boost::get< int32_t > (value)));
	} else if (value.type () == typeid (uint32_t)) {
		hash.insert (QString::fromLatin1 (key.c_str ()),
		             QVariant (boost::get< uint32_t > (value)));
	} else {
		QString val;
		if (key == "url") {
			QString tmp = QString::fromUtf8 (boost::get< std::string >(value).c_str ());
#if 0
			val = decodeXmmsUrl (tmp);
#else
			tmp = decodeXmmsUrl (tmp);
			val = tmp.mid (tmp.lastIndexOf ("/") + 1);
			if (val.isEmpty ()) {
				val = tmp;
			}
#endif
		} else {
			val = QString::fromUtf8 (boost::get< std::string > (value).c_str ());
		}

		hash.insert (stdToQ (key), QVariant (val));
	}
}

/**
 * convert a Xmms::PropDict to a QHash<QString, QVariant>
**/
QHash<QString, QVariant>
XClient::convert_propdict (const Xmms::PropDict &dict)
{
	QHash<QString, QVariant> hash;
#ifdef SOURCEPREF_HACK
	MyPropDict d (dict);
	QList<QRegExp> priolist = d.getSourcePreference ();
	QHash<QString, int> curr_prio;
#endif
	dict.each (boost::bind (&XClient::propDictToQHash,
							_1, _2, _3,
#ifdef SOURCEPREF_HACK
	                        boost::ref (priolist),
	                        boost::ref (curr_prio),
#endif
							boost::ref (hash)));

	return hash;
}



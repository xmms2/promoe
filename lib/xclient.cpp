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

/*
XSettings::XSettings (QObject *parent) : QObject (parent)
{
//	* dummy *
}

void
XSettings::change_settings ()
{
	emit settingsChanged ();
}
*/

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
	m_name = name;
}

void XClient::disconnect ()
{
	delete m_client;
	m_client = NULL;
	m_isconnected = false;

	emit disconnected (this);
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
		if (key == "url") {
			/* This really is wrong ...*/
			char *c = const_cast<char *>(xmmsc_result_decode_url (NULL, boost::get< std::string >(value).c_str ()));
			val = QString::fromUtf8 (c);
			val = val.mid (val.lastIndexOf ("/") + 1);
			if (val.isEmpty ()) {
				val = QString::fromUtf8 (c);
			}
			free (c);
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
	dict.each (boost::bind (&XClient::propDictToQHash,
							_1, _2, _3, boost::ref (hash)));

	return hash;
}



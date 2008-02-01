/**
 *	This file is a part of Promoe, an XMMS2 Client
 *
 *	Copyright (C) 2007,2008 Thomas Frauendorfer
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version 2
 *	of the License, or (at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 */

#include <xmmsclient/xmmsclient++.h>

#include "xsettings.h"

#include <QList>

XSettings::XSettings (QObject *parent, XClient *client) : QObject (parent)
{
	connect (client, SIGNAL (gotConnection (XClient *)),
	         this, SLOT (on_connect (XClient *)));

	connect (client, SIGNAL (disconnected (XClient *)),
	         this, SLOT (on_disconnect (XClient *)));

	if (client->isConnected ()) {
		on_connect (client);
	}
}

QString
XSettings::value_get (QString key)
{
	/* if XSettings is ready, the local cache should be in sync with the
	 * serverside settings, otherwise the cache is empty */
	if (!m_ready) {
		return QString ();
	}
	return m_config_cache.value (key);
}

bool
XSettings::value_set (QString key, QString val)
{
	/* Only send change request to server here
	 * update of local cache will be done through handle_config_value_changed
	 */
	if (!m_client->isConnected ()) {
		return false;
	}
	m_client->config ()->valueSet (key.toStdString (), val.toStdString ());

	return true;
}

bool
XSettings::value_register (QString key, QString defval)
{
	if (!m_client->isConnected ()) {
		return false;
	}
	m_client->config ()->valueRegister (key.toStdString (),
	                                    defval.toStdString ());

	return true;
}

void
XSettings::on_connect (XClient *client)
{
	client->config ()->valueList ()
	        (Xmms::bind (&XSettings::handle_config_value, this));

	client->config ()->broadcastValueChanged ()
	        (Xmms::bind (&XSettings::handle_config_value_changed, this));

	m_client = client;
}

void
XSettings::on_disconnect (XClient *client)
{
	/* We don't emit any signals here, as every class must be able to
	 * react on the configChanged signal, which will be fired for every
	 * configvalue on reonnect
	 */
	m_ready = false;
	m_config_cache.clear ();
}

bool
XSettings::handle_config_value (const Xmms::Dict &value)
{
	bool ok = handle_config_value_changed (value);
	if (ok) {
		m_ready = true;
	}
	return ok;
}

bool
XSettings::handle_config_value_changed (const Xmms::Dict &value)
{
	QHash <QString, QVariant> tmp = XClient::convert_dict(value); 

	QHash<QString, QVariant>::const_iterator i = tmp.constBegin ();
	while (i != tmp.constEnd ())
	{
		m_config_cache[i.key ()] = i.value ().toString ();
		emit configChanged (i.key (), i.value ().toString ());
		i++;
	}

	// TODO
	return true;
}

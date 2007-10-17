/**
 *	This file is a part of Promoe, an XMMS2 Client
 *
 *	Copyright (C) 2007 Thomas Frauendorfer
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
	connect (client, SIGNAL(gotConnection (XClient *)),
	         this, SLOT (got_connection (XClient *)));

	if (client->isConnected ()) {
		got_connection (client);
	}
}

QString
XSettings::value_get (QString key)
{
	/* local cache should be identical to serverside config */
	return m_config_cache.value (key);
}

void
XSettings::value_set (QString key, QString val)
{
	/* Only send change request to server here
	 * update of local cache will be done through handle_config_value_changed
	 */
	m_client->config ()->valueSet (key.toStdString (), val.toStdString ());
}

void
XSettings::value_register (QString key, QString defval)
{
	m_client->config ()->valueRegister (key.toStdString (),
	                                    defval.toStdString ());
}

void
XSettings::got_connection (XClient *client)
{
	client->config ()->valueList ()
	        (Xmms::bind (&XSettings::handle_config_value_changed, this));

	client->config ()->broadcastValueChanged ()
	        (Xmms::bind (&XSettings::handle_config_value_changed, this));

	m_client = client;
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

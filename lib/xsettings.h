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

#ifndef __XSETTINGS__
#define __XSETTINGS__

class XSettings;

#include "xclient.h"

#include <QObject>
#include <QHash>
#include <QString>

class XSettings : public QObject
{
	Q_OBJECT
	public:
		XSettings (QObject *parent, XClient *client);

		QString value_get (QString key);
		void value_set (QString key, QString value);
		void value_register (QString key, QString defval);

	signals:
		void configChanged(QString key, QString value);

	public slots:
		void got_connection(XClient *);

	private:
		bool handle_config_value_changed (const Xmms::Dict &value);

		QHash < QString, QString > m_config_cache;
		XClient *m_client;
};

#endif

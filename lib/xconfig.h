/**
 *	This file is a part of Promoe, an XMMS2 Client
 *
 *	Copyright (C) 2005-2008 XMMS2 Team
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

#ifndef __XConfig_H__
#define __XConfig_H__

class XClient;
namespace Xmms {
	class Dict;
}

#include <QObject>
#include <QHash>
#include <QRegExp>
#include <QString>
#include <QStringList>

class XConfig : public QObject
{
	Q_OBJECT
	public:
		XConfig (XClient *parent);

		QString value_get (QString key);
		QStringList values_get (QRegExp key = QRegExp (".*"));
		bool value_set (QString key, QString value);
		bool value_register (QString key, QString defval);

		bool isReady (void) {return m_ready;}

	signals:
		void configChanged (QString key, QString value);

	public slots:
		void on_connect (XClient *);
		void on_disconnect (XClient *);

	private:
		bool handle_config_value (const Xmms::Dict &value);
		bool handle_config_value_changed (const Xmms::Dict &value);

		bool m_ready;
		QHash < QString, QString > m_config_cache;
		XClient *m_client;
};

#endif

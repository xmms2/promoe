/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef __SERVER_BROWSER_H__
#define __SERVER_BROWSER_H__

#include "xclient.h"

#include <QWidget>
#include <QListWidget>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>

#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-qt4/qt-watch.h>

class ServerItem : public QListWidgetItem {
	public:
		ServerItem (QIcon i, const QString &name, const QString &path, QListWidget *parent) :
			QListWidgetItem (i, name, parent) {
				m_path = path;
				setToolTip (m_path);
			}
		QString path (void) const { return m_path; }
	private:
		QString m_path;
};

class ServerBrowserList : public QListWidget {
	Q_OBJECT
	public:
		ServerBrowserList (QWidget *parent);
		~ServerBrowserList ();
		void addServer (const QString &, const QString &);
		bool setupAvahi (void);
		AvahiClient *client (void) const { return m_client; }

	public slots:
		void connectServer (QListWidgetItem *);

	private:
		const AvahiPoll *m_poll;
		AvahiClient *m_client;
		AvahiServiceBrowser *m_sb;
};

class ServerBrowserWindow : public QMainWindow {
	Q_OBJECT
	public:
		ServerBrowserWindow (QWidget *parent);
		~ServerBrowserWindow () {}
	private:
		ServerBrowserList *m_list;
};

#endif

/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
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

#include "ServerBrowser.h"
#include "application.h"

#include <QPushButton>

ServerBrowserWindow::ServerBrowserWindow (QWidget *parent) : QMainWindow (parent)
{
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif
	setWindowFlags (Qt::Dialog);
	setWindowModality (Qt::ApplicationModal);
	setAttribute (Qt::WA_DeleteOnClose);

	QWidget *c = new QWidget (this);
	setCentralWidget (c);

	QVBoxLayout *vbox = new QVBoxLayout (c);
	QLabel *label = new QLabel ("Available XMMS2 servers...", c);
	label->setFont (QFont ("Helvetica", 16));
	vbox->addWidget (label);

	m_list = new ServerBrowserList (c);
	vbox->addWidget (m_list);

	c = new QWidget (c);
	QHBoxLayout *hbox = new QHBoxLayout (c);
	vbox->addWidget (c);

	QPushButton *connect = new QPushButton ("Connect", c);
	hbox->addWidget (new QWidget (), 1);
	hbox->addWidget (connect);

	resize (300, 400);
}

ServerBrowserList::ServerBrowserList (QWidget *parent) : QListWidget (parent)
{
	setIconSize (QSize (32, 32));
	setupAvahi ();


	connect (this, SIGNAL (itemDoubleClicked (QListWidgetItem *)),
			 SLOT (connectServer (QListWidgetItem *)));
}

void
ServerBrowserList::connectServer (QListWidgetItem *it)
{
	ServerBrowserWindow *sw = dynamic_cast<ServerBrowserWindow*> (window ());

	XClient *client = App->client ();
	ServerItem *item = dynamic_cast<ServerItem*> (it);
	if (client->connect (item->path ().toAscii())) {
		sw->close ();
	}
}

void
ServerBrowserList::addServer (const QString &name, const QString &path)
{
	new ServerItem (QIcon (":icon.png"), name, path, this);
	update ();
}

static void
resolve_callback (AvahiServiceResolver *r,
				  AvahiIfIndex interface,
				  AvahiProtocol protocol,
				  AvahiResolverEvent event,
				  const char *name,
				  const char *type,
				  const char *domain,
				  const char *host_name,
				  const AvahiAddress *address,     
				  uint16_t port,
				  AvahiStringList *txt,
				  AvahiLookupResultFlags flags,
				  void* userdata)
{
	ServerBrowserList *sl = static_cast<ServerBrowserList*> (userdata);

	switch (event) {
		case AVAHI_RESOLVER_FAILURE:
			qWarning ("something broke...");
			break;
		case AVAHI_RESOLVER_FOUND:
			char a[AVAHI_ADDRESS_STR_MAX];
			avahi_address_snprint (a, sizeof (a), address);
			QString path;
			path.sprintf ("tcp://%s:%u", a, port);

			sl->addServer (QString::fromLatin1 (name), path);
			break;
	}
}

static void
browse_callback (AvahiServiceBrowser *b,
				 AvahiIfIndex interface,
				 AvahiProtocol protocol,
				 AvahiBrowserEvent event,
				 const char *name,
				 const char *type,
				 const char *domain,
				 AvahiLookupResultFlags flags,
				 void* userdata) 
{
	ServerBrowserList *sl = static_cast<ServerBrowserList*> (userdata);

	switch (event) {
		case AVAHI_BROWSER_FAILURE:

			qWarning("(Browser) %s\n", avahi_strerror (avahi_client_errno(avahi_service_browser_get_client(b))));
			sl->close ();
			return;

		case AVAHI_BROWSER_NEW:
			if (!(avahi_service_resolver_new (sl->client (), interface,
											  protocol, name,
											  type, domain,
											  AVAHI_PROTO_UNSPEC,
											  (AvahiLookupFlags) 0, 
											  resolve_callback, userdata)))
			{
				qWarning ("couldn't resolv service!");
				sl->close ();
			}
			break;

		case AVAHI_BROWSER_REMOVE:
			break;
		case AVAHI_BROWSER_ALL_FOR_NOW:
			qDebug ("done!");
			break;
		case AVAHI_BROWSER_CACHE_EXHAUSTED:
			break;
	}
}

static void
client_callback (AvahiClient *c,
				 AvahiClientState state,
				 void *userdata)
{
	ServerBrowserList *sl = static_cast<ServerBrowserList*> (userdata);

	if (state == AVAHI_CLIENT_FAILURE) {
		qWarning ("Avahi failure!!");
		sl->close ();
	}
}

bool
ServerBrowserList::setupAvahi (void)
{
	int ret = 1;
	int error;

	m_poll = avahi_qt_poll_get ();
	m_client = avahi_client_new (m_poll, (AvahiClientFlags)0, client_callback, this, &error);

	if (!m_client) {
		qWarning ("couldn't create avahi browser!");
		return false;
	}

	if (!(m_sb = avahi_service_browser_new (m_client, AVAHI_IF_UNSPEC,
											AVAHI_PROTO_UNSPEC,
											"_xmms2._tcp", NULL, (AvahiLookupFlags)0,
											browse_callback, this))) {
		qWarning ("couldn't create avahi browser!");
		return false;
	}

}

ServerBrowserList::~ServerBrowserList ()
{
	/*
	if (m_poll) {
		delete m_poll;
	}
	if (m_client) {
		avahi_client_free (m_client);
	}
	if (m_sb) {
		avahi_service_browser_free (m_sb);
	}
	*/
}

#include "ServerBrowser.moc"

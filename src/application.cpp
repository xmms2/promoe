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

#include "xclient.h"

#include "application.h"

#include "mainwindow.h"
#include "equalizerwindow.h"
#include "playlistwindow.h"
#include "Skin.h"

#ifdef HAVE_SERVERBROWSER
#include "Serverbrowser.h"
#endif

#include <QSettings>
#include <QMessageBox>

Application::Application (int &argc, char **argv) : QApplication (argc, argv)
{
	setOrganizationName("xmms2");
	setOrganizationDomain("xmms.org");
	setApplicationName("Promoe");

	m_xmms2_client = new XClient (this, "Promoe");

	connect (this, SIGNAL (aboutToQuit ()),
	         this, SLOT (cleanupHandler ()));
	connect (m_xmms2_client, SIGNAL(disconnected(XClient *)),
	         this, SLOT(handleDisconnected ()));

	QSettings settings;

	MainWindow *mw = new MainWindow (NULL);

	/*
	 * Now that everything is initialized
	 * open the skin and send the
	 * SkinChanged signal that will cause
	 * all widgets to get their pixmaps
	 * TODO: Now the Skin class loads the skin itself. This call is necessarry
	 * until all widgets that receive the skinChanged signal fetch their
	 * pixmaps themself on startup
	 */
	Skin::getInstance()->setSkin (settings.value("skin/path").toString ());

	mw->show ();
	// The Playlist- and EqualizerWindow has to become visible after the
	// mainwindow because metacity (gnome-windowmanager) can't handle them
	//  correctly otherwise
	mw->getEQ ()->setVisible (settings.value ("equalizer/visible", false).toBool ());
	mw->getPL ()->setVisible (settings.value ("playlist/visible", false).toBool ());

	m_xmms2_client->connect ();

#ifdef HAVE_SERVERBROWSER
	ServerBrowserWindow *browser = new ServerBrowserWindow (mw);
	browser->show ();
#endif

	m_timemode_reverse = settings.value ("MainWindow/timemodereverse",
	                                      false).toBool();
}

void
Application::cleanupHandler ()
{
	QSettings s;

	s.setValue("MainWindow/timemodereverse",m_timemode_reverse);

	if (s.value ("promoe/quitonclose", false).toBool ())
		m_xmms2_client->shutdownServer ();
}

void
Application::handleDisconnected ()
{
	// if the Application is about to quit, we no longer need to handle
	// disconnects
	if (closingDown ())
		return;

	// TODO: enable reconnect
	QMessageBox::critical( NULL, "xmms2 daemon disconnected",
	                      "The xmms2 deamon has disconnected\n"
	                      "This could be because the server crashed\n"
	                      "or because another client has shut down the sever.",
	                      "Quit Promoe");
	QApplication::quit ();
}

int
main (int argc, char **argv)
{
	Application app(argc, argv);

#ifdef Q_OS_MACX
	/** This is soooo wrong, there must exsist a flag for
	  * static plugins
	Q_IMPORT_PLUGIN(QJpegPlugin);
	  */
#endif


	return app.exec();
}

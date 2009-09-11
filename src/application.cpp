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
	 * After all windows have been initialized, send the skinChanged signal,
	 * so that all windows fetch their skin information.
	 * The windows should fetch their skin information themselfes on startup
	 * This is a wokaround until all widgets have been fixed
	 */
	Skin::getInstance()->emitSkinChanged();

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

	int pressed =
	QMessageBox::warning( NULL, "xmms2 daemon disconnected",
	                      "The xmms2 daemon has disconnected\n"
	                      "This could be because the server crashed\n"
	                      "or because another client has shut down the sever.",
	                      "Reconnect", "Quit Promoe");
	switch (pressed)
	{
		case 0:
			m_xmms2_client->connect ();
			break;
		default:
			QApplication::quit ();
	}
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

#include "application.moc"

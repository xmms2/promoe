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

#include "XMMSHandler.h"

#include "application.h"

#include "mainwindow.h"
#include "Skin.h"

#ifdef HAVE_SERVERBROWSER
#include "Serverbrowser.h"
#endif

#include <QSettings>
#include <QMessageBox>

Application::Application (int &argc, char **argv) : QApplication (argc, argv)
{
	m_want_quit = false;

	//TODO: Change to XClient sometime later
	XMMSHandler &client = XMMSHandler::getInstance ();

	connect (&client, SIGNAL(disconnected(XClient *)),
	         this, SLOT(handleDisconnected ()));
}

void
Application::quit ()
{
	// quit for real in case something went wrong earlier
	if (m_want_quit)
		QApplication::quit ();

	QSettings s;
	if (s.value ("promoe/quitonclose", false).toBool ()) {
		m_want_quit = true;
		if (!XMMSHandler::getInstance ().quit ()) {
			QApplication::quit ();
		}
	} else {
		QApplication::quit ();
	}
}

void
Application::handleDisconnected ()
{
	if (!m_want_quit) {
	// TODO: enable reconnect
		QMessageBox::critical( NULL, "xmms2 daemon disconnected",
		                      "The xmms2 deamon has disconnected\n"
		                      "This could be because the server crashed\n"
		                      "or because another client has shut down the sever.",
		                      "Quit Promoe");
	}
	QApplication::quit ();
}

int 
main (int argc, char **argv)
{
	Application app(argc, argv);

	QCoreApplication::setOrganizationName("xmms2");
	QCoreApplication::setOrganizationDomain("xmms.org");
	QCoreApplication::setApplicationName("Promoe");

	QSettings settings;

#ifdef Q_OS_MACX 
	/** This is soooo wrong, there must exsist a flag for
	  * static plugins
	Q_IMPORT_PLUGIN(QJpegPlugin);
	  */
#endif

	MainWindow *mw = new MainWindow (NULL);

	/*
	 * Now that everything is initialized
	 * open the skin and send the
	 * SkinChanged signal that will cause
	 * all widgets to get their pixmaps
	 */
	if (!settings.contains ("skin/path")) {
		settings.setValue ("skin/path", ":CleanAMP/");
	}

	Skin::getInstance()->setSkin (settings.value("skin/path").toString ());

	mw->show ();

#ifdef HAVE_SERVERBROWSER
	ServerBrowserWindow *browser = new ServerBrowserWindow (mw);
	browser->show ();
#endif

	return app.exec();
}

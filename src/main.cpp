/**
 *  This file is a part of Prome, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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

#include "MainWindow.h"
#include "playlist.h"
#include "equalizer.h"
#include "Skin.h"
//TODO: change to xclient.h sometime later
#include "XMMSHandler.h"

#ifdef HAVE_SERVERBROWSER
#include "Serverbrowser.h"
#endif

#include <QSettings>

int 
main (int argc, char **argv)
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("xmms2");
	QCoreApplication::setOrganizationDomain("xmms.org");
	QCoreApplication::setApplicationName("Promoe");

	//TODO: Change to XClient sometime later
	XMMSHandler &client = XMMSHandler::getInstance ();

	QSettings settings;

#ifdef Q_OS_MACX 
	/** This is soooo wrong, there must exsist a flag for
	  * static plugins
	Q_IMPORT_PLUGIN(QJpegPlugin);
	  */
#endif

	MainWindow *mw = new MainWindow (NULL);

	PlaylistWindow *playlistwin = new PlaylistWindow (mw);
	EqualizerWindow *eqwin = new EqualizerWindow (mw);

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
	mw->setPL (playlistwin);
	mw->setEQ (eqwin);

	if (!settings.contains ("playlist/pos"))
		settings.setValue ("playlist/pos", QPoint (mw->pos().x(),
												   mw->pos().y()+mw->size().height()));
	playlistwin->move (settings.value("playlist/pos").toPoint ());

	if (!settings.contains ("playlist/hidden"))
		settings.setValue ("playlist/hidden", true);

	if (settings.value("playlist/hidden").toBool ())
		playlistwin->hide ();
	else
		playlistwin->show ();


	if (!settings.contains ("equalizer/pos"))
		settings.setValue ("equalizer/pos", QPoint (mw->pos().x(),
												   mw->pos().y()+mw->size().height()));
	eqwin->move (settings.value("equalizer/pos").toPoint ());

	if (!settings.contains ("equalizer/hidden"))
		settings.setValue ("equalizer/hidden", true);

	if (settings.value("equalizer/hidden").toBool ())
		eqwin->hide ();
	else
		eqwin->show ();


#ifdef HAVE_SERVERBROWSER
	ServerBrowserWindow *browser = new ServerBrowserWindow (mw);
	browser->show ();
#endif

	return app.exec();
}

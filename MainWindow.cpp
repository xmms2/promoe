#include <xmmsclient/xmmsclient++.h>
#include "MainWindow.h"

#ifdef HAVE_SERVERBROWSER
#include "ServerBrowser.h"
#endif

#include <QSettings>
#include <QIcon>
#include <QPluginLoader>
#include <qplugin.h>

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
	QSettings settings;

	setWindowFlags(Qt::FramelessWindowHint);
	setGeometry(100, 100, 275, 116);
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif

	if (!settings.contains ("mainwindow/shaded"))
		setShaded (true);
	else
		setShaded (!isShaded ());

	/* 
	 * The MainDisplay is the mainwindow non-shaded mode
	 */
	m_display = new MainDisplay (this);
	setCentralWidget (m_display);
	m_display->show ();

	/*
	 * MainDisplay's shaded mode
	 */
	m_shaded = new ShadedDisplay (this);
	m_shaded->hide ();

	switchDisplay ();

	m_playlistwin = NULL;

	if (!settings.contains ("mainwindow/pos"))
		settings.setValue ("mainwindow/pos", QPoint (100, 100));

	move (settings.value("mainwindow/pos").toPoint ());
}

MainWindow::~MainWindow ()
{
	delete Skin::getInstance ();
	delete XMMSHandler::getInstance();
}

void
MainWindow::switchDisplay ()
{
	QSettings s;

	if (isShaded ()) {
		m_shaded->hide ();
		m_display->show ();
		resize (275, 116);
		setShaded (false);
	} else {
		m_display->hide ();
		m_shaded->show ();
		resize (275, 14);
		setShaded (true);
	}

	update ();

}

void
MainWindow::moveEvent (QMoveEvent *event)
{
	int x = event->oldPos().x();
	int y = event->oldPos().y() + size().height();

	if (!m_playlistwin)
		return;
	
	int x2 = m_playlistwin->pos().x();
	int y2 = m_playlistwin->pos().y();

	int diffy = y - y2;
	int diffx = x - x2;

	if ((diffy > -5 && diffy < 5) && (diffx > -10 && diffx < 10))
	{
		m_playlistwin->move (event->pos().x(),
							 event->pos().y() + size().height());
	}

	QSettings s;
	s.setValue ("mainwindow/pos", pos ());
}

void 
MainWindow::togglePL (bool UpdateButton) 
{ 
	QSettings s;

	if(UpdateButton)
	{
		getMD()->GetPls()->toggleOn();
	}

	if (s.value ("playlist/hidden").toBool ()) {
		m_playlistwin->move (s.value("playlist/pos").toPoint ());
		m_playlistwin->show (); 
		s.setValue ("playlist/hidden", false);
	} else {
		m_playlistwin->hide (); 
		s.setValue ("playlist/hidden", true);
	} 
}


int 
main (int argc, char **argv)
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("XMMS2 Team");
	QCoreApplication::setOrganizationDomain("xmms.org");
	QCoreApplication::setApplicationName("Promoe");

	QSettings settings;

#ifdef Q_OS_MACX 
	/** This is soooo wrong, there must exsist a flag for
	  * static plugins
	  */
	Q_IMPORT_PLUGIN(QJpegPlugin);
#endif

	MainWindow *mw = new MainWindow (NULL);

	PlaylistWindow *playlistwin = new PlaylistWindow (mw);

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

	if (!settings.contains ("playlist/pos"))
		settings.setValue ("playlist/pos", QPoint (mw->pos().x(),
												   mw->pos().y()+mw->size().height()));
	playlistwin->move (settings.value("playlist/pos").toPoint ());
	playlistwin->move (settings.value("playlist/pos").toPoint ());

	if (!settings.contains ("playlist/hidden"))
		settings.setValue ("playlist/hidden", true);

	if (settings.value("playlist/hidden").toBool ())
		playlistwin->hide ();
	else
		playlistwin->show ();

#ifdef HAVE_SERVERBROWSER
	ServerBrowserWindow *browser = new ServerBrowserWindow (mw);
	browser->show ();
#endif

	return app.exec();
}

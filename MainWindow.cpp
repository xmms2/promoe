#include <xmmsclient/xmmsclient++.h>
#include "MainWindow.h"

#include <QSettings>
#include <QIcon>
#include <QPluginLoader>
#include <qplugin.h>

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setGeometry(100, 100, 275, 116);
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif

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

	/*
	 * Per default not shaded. Change this to a 
	 * config value later.
	 */
	m_isshaded = false;

	m_playlistwin = NULL;

}

MainWindow::~MainWindow ()
{
	delete Skin::getInstance ();
	delete XMMSHandler::getInstance();
}

void
MainWindow::switchDisplay ()
{
	if (m_isshaded) {
		m_shaded->hide ();
		m_display->show ();
		resize (275, 116);
		m_isshaded = false;
	} else {
		m_display->hide ();
		m_shaded->show ();
		resize (275, 14);
		m_isshaded = true;
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
}

void 
MainWindow::togglePL (void) 
{ 
	if (m_playlistwin->isVisible ()) { 
		m_playlistwin->hide (); 
	} else { 
		m_playlistwin->move (pos().x(), pos().y()+size().height());
		m_playlistwin->show (); 
	} 
}


int 
main (int argc, char **argv)
{
	QApplication app(argc, argv);
	QSettings settings ("XMMS2", "Promoe");

	Q_IMPORT_PLUGIN(QJpegPlugin);

	MainWindow *mw = new MainWindow (NULL);

	PlaylistWindow *playlistwin = new PlaylistWindow (NULL);

	/*
	 * Now that everything is initialized
	 * open the skin and send the
	 * SkinChanged signal that will cause
	 * all widgets to get their pixmaps
	 */
	if (!settings.contains ("skin/path")) {
		settings.setValue ("skin/path", "./CleanAMP/");
	}

	Skin::getInstance()->setSkin (settings.value("skin/path").toString ());

	mw->show ();
	mw->setPL (playlistwin);

	return app.exec();
}

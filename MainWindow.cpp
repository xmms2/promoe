#include <xmmsclient/xmmsclient++.h>
#include "MainWindow.h"

#include <QSettings>

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setGeometry(100, 100, 275, 116);
	
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

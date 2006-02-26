#include <xmmsclient/xmmsclient++.h>
#include "MainWindow.h"

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setGeometry(100, 100, 275, 116);
	
	/* 
	 * Initialize the Handler that will
	 * update the display and the buttons
	 */
	m_handler = XMMSHandler::getInstance();

	/*
	 * Initialize skin, but don't open one
	 */
	skin = new Skin ();
	
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
	delete skin;
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

Skin *MainWindow::getSkin(void)
{
	return skin;
}

int 
main (int argc, char **argv)
{
	QApplication app(argc, argv);

	MainWindow *mw = new MainWindow (NULL);

	PlaylistWindow *playlistwin = new PlaylistWindow (NULL, mw->getSkin ());

	/*
	 * Now that everything is initialized
	 * open the skin and send the
	 * SkinChanged signal that will cause
	 * all widgets to get their pixmaps
	 */
	mw->getSkin ()->setSkin ("./CleanAMP/");
	mw->show ();
	mw->setPL (playlistwin);

	return app.exec();
}

#include <xmmsclient/xmmsclient++.h>
#include "MainWindow.h"

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setGeometry(100, 100, 275, 116);
	
	m_handler = new XMMSHandler (this);

	skin = new Skin ();
	m_display = new MainDisplay(this);
	
	setCentralWidget (m_display);

	skin->setSkin ("./CleanAMP/");
	m_display->show ();

}

MainWindow::~MainWindow ()
{
	delete skin;
}

void
MainWindow::setNoDrag (bool b)
{
	m_display->setNoDrag (b);
}

Skin *MainWindow::getSkin(void)
{
	return skin;
}

int main (int argc, char **argv)
{
	QApplication app(argc, argv);

	MainWindow *mw = new MainWindow (NULL);
	mw->show();
	

	return app.exec();
}

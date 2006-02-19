#include <xmmsclient/xmmsclient++.h>
#include "MainWindow.h"

MainWindow::MainWindow ()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setGeometry(100, 100, 275, 116);
	
	m_handler = new XMMSHandler (this);

	skin = new Skin("./Debian/");
	m_display = new MainDisplay(this);
	
	setCentralWidget(m_display);

	skin->setSkin("./Debian/");
	m_display->show();

}

MainWindow::~MainWindow ()
{
	delete skin;
}

Skin *MainWindow::getSkin(void)
{
	return skin;
}

int main (int argc, char **argv)
{
	QApplication app(argc, argv);

	MainWindow *mw = new MainWindow();
	mw->show();
	

	return app.exec();
}

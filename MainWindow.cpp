#include "MainWindow.h"

MainWindow::MainWindow ()
{
	setWindowFlags(Qt::FramelessWindowHint);
	setGeometry(100, 100, 275, 116);

	skin = new Skin("Debian");
	MainDisplay *display = new MainDisplay(this);
	setCentralWidget(display);

	skin->setSkin("./CleanAMP/");
	display->show();
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

	/*
	xmmsc_connection_t *xmmsc = xmmsc_init ("coolness");
	xmmsc_connect (xmmsc, "unix:///tmp/xmms-ipc-nano");

	XmmsQT4 *c = new XmmsQT4(xmmsc, &app);*/
	MainWindow *mw = new MainWindow();
	mw->show();

	return app.exec();
}

#include "XMMSSocket.h"
#include "MedialibWindow.h"
#include "MedialibView.h"

#include <QMainWindow>

MedialibWindow::MedialibWindow (QWidget *parent) : QMainWindow (parent)
{
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif

	setWindowFlags (Qt::Dialog);
	setAttribute (Qt::WA_DeleteOnClose);

	m_view = new MedialibView (this);

	setCentralWidget (m_view);

	resize (500, 600);
}

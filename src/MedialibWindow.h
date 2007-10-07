#ifndef __MEDIALIBWINDOW_H__
#define __MEDIALIBWINDOW_H__

#include "MedialibView.h"
#include <QMainWindow>

class MedialibWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MedialibWindow (QWidget *parent);

	private:
		QTreeView *m_view;
};

#endif

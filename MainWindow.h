#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <iostream>

#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QHash>

#include <xmmsclient/xmmsclient.h>

#include "Skin.h"
#include "XmmsQT4.h"


using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow(void);
		~MainWindow(void);
		Skin *getSkin(void);
	private:
		Skin *skin;
};

#endif

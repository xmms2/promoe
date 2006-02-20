#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

class MainWindow;

#include "XMMSHandler.h"

#include <iostream>

#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QHash>

#include "Skin.h"
#include "XmmsQT4.h"
#include "MainDisplay.h"

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow (QWidget *parent);
		~MainWindow (void);
		Skin *getSkin (void);
		XMMSHandler *getHandler () { return m_handler; }
		MainDisplay *getMD () { return m_display; }
		void setNoDrag (bool b);
	private:
		Skin *skin;
		XMMSHandler *m_handler;
		MainDisplay *m_display;
};

#endif

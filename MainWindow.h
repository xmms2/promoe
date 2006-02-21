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
#include "ShadedDisplay.h"

using namespace std;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow (QWidget *parent);
		~MainWindow (void);

		Skin *getSkin (void);
		void setNoDrag (bool b);

		XMMSHandler *getHandler () { return m_handler; }
		MainDisplay *getMD () { return m_display; }
		ShadedDisplay *getSD () { return m_shaded; }

		bool getShaded (void) { return m_isshaded; }

	public slots:
		void switchDisplay ();

	private:
		bool m_isshaded;
		Skin *skin;

		XMMSHandler *m_handler;
		MainDisplay *m_display;
		ShadedDisplay *m_shaded;
};


#endif

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
#include <QSettings>

#include "Skin.h"
#include "XmmsQT4.h"
#include "MainDisplay.h"
#include "ShadedDisplay.h"
#include "Playlist.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MainWindow (QWidget *parent);
		~MainWindow (void);

		MainDisplay *getMD () { return m_display; }
		ShadedDisplay *getSD () { return m_shaded; }

		void setPL (PlaylistWindow *p) { m_playlistwin = p; }
		void raisePL (void) { m_playlistwin->raise (); }
		void moveEvent (QMoveEvent *event);

	public slots:
		void switchDisplay ();
		void togglePL (void);

	private:
		bool isShaded (void) { QSettings s; return s.value("MainWindow/shaded").toBool(); }
		void setShaded (bool b) { QSettings s; return s.setValue("MainWindow/shaded", b); }
		MainDisplay *m_display;
		ShadedDisplay *m_shaded;
		PlaylistWindow *m_playlistwin;
};


#endif

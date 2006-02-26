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
#include "Playlist.h"

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
		ShadedDisplay *getSD () { return m_shaded; }

		bool getShaded (void) { return m_isshaded; }
		void setPL (PlaylistWindow *p) { m_playlistwin = p; }

	public slots:
		void switchDisplay ();
		void togglePL (void) { 
			if (m_playlistwin->isVisible ()) { 
				m_playlistwin->hide (); 
			} else { 
				m_playlistwin->show (); 
			} 
		}

	private:
		bool m_isshaded;
		Skin *skin;

		XMMSHandler *m_handler;
		MainDisplay *m_display;
		ShadedDisplay *m_shaded;
		PlaylistWindow *m_playlistwin;
};


#endif

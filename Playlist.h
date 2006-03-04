#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include "Skin.h"
#include "PlaylistList.h"
#include "PixWidget.h"
#include "Button.h"
#include "PlaylistShade.h"
#include <QMainWindow>
#include <QFont>

class PlaylistWidget;
class PlaylistScroller;

class dragButton : public Button {
	public:
		dragButton (QWidget *parent) : Button (parent) {}
		~dragButton () {}

		void mouseMoveEvent (QMouseEvent *event);
};

class PlaylistScrollButton : public Button {
	public:
		PlaylistScrollButton (PlaylistScroller *parent, uint normal, uint pressed);
		~PlaylistScrollButton () {}
	private:
		PlaylistScroller *m_slider;
		void mouseMoveEvent (QMouseEvent *event);
};

class PlaylistScroller : public QWidget{
	Q_OBJECT
	public:
		PlaylistScroller (PlaylistWidget *arent);
		~PlaylistScroller () {}
		void doScroll (int p) { emit scrolled(p); }

		int getMax (void);

	public slots:
		void setPixmaps (Skin *skin);

	signals:
		void scrolled (int);

	private:
		void paintEvent (QPaintEvent *event);
		QPixmap m_pixmap;
		PlaylistScrollButton *m_button;
};

class PlaylistView : public QWidget {
	public:
		PlaylistView (QWidget *parent) : QWidget (parent) {}
		~PlaylistView () {}
};

class PlaylistWidget : public QWidget {
	Q_OBJECT

	public:
		PlaylistWidget (QWidget *parent);
		~PlaylistWidget () {}

		void setActive (bool);
		void switchDisplay (void);

	public slots:
		void setPixmaps (Skin *skin);
		void doScroll (int);

	private:
		void resizeEvent (QResizeEvent *event);
		void paintEvent (QPaintEvent *event);
		void mouseDoubleClickEvent (QMouseEvent *event);

		QPixmap m_corner1;
		QPixmap m_corner2;
		QPixmap m_corner3;
		QPixmap m_corner4;

		QPixmap m_titlebar;
		QPixmap m_tfill;
		QPixmap m_bfill;
		QPixmap m_lfill;
		QPixmap m_rfill;
		QPixmap m_rfill2;
		QPixmap m_rfill3;

		bool m_active;
		

		PlaylistView *m_view;
		PlaylistList *m_list;
		PlaylistScroller *m_scroller;
		dragButton *m_drag;
};


class PlaylistWindow : public QMainWindow {
	Q_OBJECT

	public:
		PlaylistWindow (QWidget *parent);
		~PlaylistWindow () {}

		void setActive (bool);
		void switchDisplay (void);

		void mousePressEvent (QMouseEvent *event);
		void mouseMoveEvent (QMouseEvent *event);
		void enterEvent (QEvent *event);
		void leaveEvent (QEvent *event);
		void resizeEvent (QResizeEvent *event);

	private:
		PlaylistWidget *m_playlist;
		PlaylistShade *m_shaded;
		
		int m_diffx;
		int m_diffy;

};

#endif

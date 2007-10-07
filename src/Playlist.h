#ifndef __PLAYLIST_H__
#define __PLAYLIST_H__

#include "XMMSHandler.h"

#include <QMainWindow>
#include <QFont>
#include <QScrollBar>

class MainWindow;
class PlaylistWidget;
class PlaylistScroller;

#include "Button.h"

class Skin;
//class PlaylistList;
class PlaylistView;
class PlaylistShade;
class PlaylistMenu;



class PlaylistScrollBar : public QScrollBar {
	Q_OBJECT

	public:
		PlaylistScrollBar (QWidget *parent = NULL);
		~PlaylistScrollBar () {}

	public slots:
		void mouseMoveEvent (QMouseEvent *event);
		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);
		void paintEvent (QPaintEvent *event);
		void setPixmaps (Skin *skin);

	private:
		int sliderPositionFromValue ();
		int sliderValueFromPosition (int position);

		int m_sliderOffset;
		QPixmap m_pixmap;
		QPixmap m_slider;
		QPixmap m_slider_down;
};


class dragButton : public Button {
	public:
		dragButton (QWidget *parent) : Button (parent) {}
		~dragButton () {}

		void mouseMoveEvent (QMouseEvent *event);
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

		void menuAddUrl () {}
		void menuAddDir ();
		void menuAddFile ();

	private:
		void resizeEvent (QResizeEvent *event);
		void paintEvent (QPaintEvent *event);
		void mouseDoubleClickEvent (QMouseEvent *event);
		void addButtons (void);
		void diveDir (const QString &);

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
		QScrollBar *m_scrollBar;
		dragButton *m_drag;

		PlaylistMenu *m_add;
		PlaylistMenu *m_del;
		PlaylistMenu *m_sel;
		PlaylistMenu *m_msc;
		PlaylistMenu *m_lst;
};


class PlaylistWindow : public QMainWindow {
	Q_OBJECT

	public:
		PlaylistWindow (QWidget *parent);
		~PlaylistWindow () {}

		void setActive (bool);

		void mousePressEvent (QMouseEvent *event);
		void mouseMoveEvent (QMouseEvent *event);
		void enterEvent (QEvent *event);
		void leaveEvent (QEvent *event);
		void moveEvent (QMoveEvent *event);
		void resizeEvent (QResizeEvent *event);

	public slots:
		void switchDisplay (void);
		void togglePL (void);

	private:
		PlaylistWidget *m_playlist;
		PlaylistShade *m_shaded;

		int m_diffx;
		int m_diffy;

		Button *m_shadebtn;
		Button *m_closebtn;
		uint getOffset (void);
		MainWindow *m_mw;

};

#endif

#ifndef __PLAYLISTMENU_H__
#define __PLAYLISTMENU_H__

class PlaylistMenu;

#include "XMMSHandler.h"
#include "Skin.h"
#include "PixWidget.h"

/*
#include <QWidget>
#include <QPixmap>
*/

class PlaylistMenu;

class PlaylistMenuBar : public PixWidget {
	Q_OBJECT
	public:
		PlaylistMenuBar (QWidget *parent, uint id) : PixWidget (parent)
		{
			m_id = id;
		}
	public slots:
		void setPixmaps (Skin *skin) { m_pixmap = skin->getPls (m_id); }

	private:
		uint m_id;
};

class PlaylistMenuButton : public QWidget {
	Q_OBJECT
	public:
		PlaylistMenuButton (PlaylistMenu *, uint pix1, uint pix2);
		~PlaylistMenuButton ();

		void paintEvent (QPaintEvent *event);
		void setPixmaps (Skin *skin);

		void setOver (void) { m_pixmap = m_pixmap2; }
		void setNoOver (void) { m_pixmap = m_pixmap1; }
		void clicked (void) { emit activated (); }

	signals:
		void activated (void);

	private:
		uint m_pixid1;
		uint m_pixid2;

		PlaylistMenu *m_menu;

		QPixmap m_pixmap1;
		QPixmap m_pixmap2;
		QPixmap m_pixmap;
};

class PlaylistMenu : public PixWidget {
	Q_OBJECT
	public:
		PlaylistMenu (QWidget *, uint, uint);
		~PlaylistMenu () {}

		void addButton (PlaylistMenuButton *b);
		void rmButton (PlaylistMenuButton *b);

		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);
		void mouseMoveEvent (QMouseEvent *);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		uint m_pixid;
		uint m_dec;
		QPixmap m_dpixmap;
		QList<PlaylistMenuButton *> m_items;
		QWidget *m_expanded;

		PlaylistMenuBar *m_decbar;

};

#endif

#ifndef __PLAYLISTLIST_H__
#define __PLAYLISTLIST_H__

#include "Skin.h"
#include <QWidget>
#include <QHash>

class PlaylistItem;

class PlaylistList : public QWidget {
	Q_OBJECT

	public:
		PlaylistList (QWidget *parent);
		~PlaylistList () {}

		void setSize (int, int);
		void addItem (PlaylistItem *i);
		void setOffset (int i) { m_offset = i; }

	public slots:
		void setPixmaps (Skin *skin);
		void playlistList (QList<uint>);
		void mediainfoChanged (uint, QHash<QString,QString>);
		void playlistChanged (QHash<QString,QString>);
		void currentID (uint);
		void setStatus (uint);
	
	private:
		void paintEvent (QPaintEvent *event);
		void mousePressEvent (QMouseEvent *event);
		void mouseDoubleClickEvent (QMouseEvent *event);
		void keyPressEvent (QKeyEvent *event);

		void mouseMoveEvent (QMouseEvent *event) {}

		QList<PlaylistItem*> *m_items;
		QList<uint> *m_selected;
		QHash<uint,PlaylistItem*> *m_itemmap;

		QFont *m_font;
		QFontMetrics *m_fontmetrics;
		QColor m_color_active;
		QColor m_color_selected;
		QColor m_color_normal;

		int getFontH (void);
		int m_offset;
		int m_active;
		uint m_status;
};

class PlaylistItem {
	public:
		PlaylistItem (PlaylistList *pl, uint);
		~PlaylistItem () {}
		
		QString text (void);

		uint getID (void) { return m_id; }
		void setText (QString s) { m_text = s; }

	private:
		uint m_id;
		PlaylistList *m_pl;

		bool m_req;
		QString m_text;
};


#endif

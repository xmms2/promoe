#ifndef __PLAYLISTLIST_H__
#define __PLAYLISTLIST_H__

#include "Skin.h"
#include <QWidget>

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
	
	private:
		void paintEvent (QPaintEvent *event);
		QList<PlaylistItem*> *m_items;
		QFont *m_font;
		QFontMetrics *m_fontmetrics;

		int getFontH (void);
		int m_offset;
};

class PlaylistItem {
	public:
		PlaylistItem (PlaylistList *pl, uint);
		~PlaylistItem () {}
		
		QString text (void);

	private:
		uint m_id;
		PlaylistList *m_pl;
};


#endif

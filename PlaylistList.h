#ifndef __PLAYLISTLIST_H__
#define __PLAYLISTLIST_H__

#include <xmmsclient/xmmsclient++.h>
#include "Skin.h"
#include <QWidget>
#include <QHash>

#include <QDrag>

class PlaylistItem;

class PlaylistList : public QWidget {
	Q_OBJECT

	public:
		PlaylistList (QWidget *parent);
		~PlaylistList () {}

		void setSize (int, int);
		void addItem (PlaylistItem *i);
		void addItem (PlaylistItem *i, uint pos);
		void setOffset (int i) { m_offset = i; }
		uint getOffset (void) const { return m_offset; }
		void doResize (void);

	public slots:
		void setPixmaps (Skin *skin);
		void playlistList (const QList<uint> &);
		void mediainfoChanged (uint, const Xmms::PropDict &);
		void playlistChanged (const Xmms::Dict &);
		void currentID (uint);
		void setStatus (Xmms::Playback::Status s);
		void settingsSaved ();
		void deleteFiles ();
	
	signals:
		void sizeChanged (QSize);
	
	private:
		void showMenu (void);
		void paintEvent (QPaintEvent *event);
		void mousePressEvent (QMouseEvent *event);
		void mouseMoveEvent (QMouseEvent *event);
		void mouseDoubleClickEvent (QMouseEvent *event);
		void keyPressEvent (QKeyEvent *event);

		void dragMoveEvent (QDragMoveEvent *event);
		void dragEnterEvent (QDragEnterEvent *event);
		void dragLeaveEvent (QDragLeaveEvent *event);
		void dropEvent (QDropEvent *event);

		QPixmap generatePixmap (int);

		QList<PlaylistItem*> *m_items;
		QList<uint> *m_selected;
		QHash<uint,PlaylistItem*> *m_itemmap;

		QFont *m_font;
		QFontMetrics *m_fontmetrics;
		QColor m_color_active;
		QColor m_color_selected;
		QColor m_color_normal;
		QColor m_color_normal_bg;

		int getFontH (void);
		int m_offset;
		int m_active;
		int m_bar;
		int m_drag_id;
		int m_pos;
		QPoint m_dragstart;
		Xmms::Playback::Status m_status;

		QDrag *m_drag;
		QMimeData *m_md;
};

class PlaylistItem {
	public:
		PlaylistItem (PlaylistList *pl, uint, uint);
		PlaylistItem (PlaylistList *pl, uint);
		~PlaylistItem () {}
		
		QString text (void);
		QString duration (void) { return m_duration; }

		uint getID (void) { return m_id; }
		void setDuration (QString s) { m_duration = s; }
		void setText (QString s) { m_text = s; }

	private:
		uint m_id;
		PlaylistList *m_pl;

		bool m_req;
		QString m_text;
		QString m_duration;
};


#endif

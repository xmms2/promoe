#ifndef __PLAYLISTVIEW_H__
#define __PLAYLISTVIEW_H__

//include "Playlist.h"
#include <xmmsclient/xmmsclient++.h>
#include "Skin.h"
#include <QObject>
#include <QAbstractItemDelegate>
#include <QListView>
#include <QWidget>

class PlaylistDelegate : public QAbstractItemDelegate {
	Q_OBJECT

	public:
		PlaylistDelegate (QObject *parent = NULL);
		~PlaylistDelegate () { }

		void paint( QPainter * painter, const QStyleOptionViewItem & option,
		            const QModelIndex & index ) const;

		QSize sizeHint ( const QStyleOptionViewItem &option,
		                 const QModelIndex &index ) const;
};


class PlaylistView : public QListView {
	Q_OBJECT

	public:
		PlaylistView (QWidget *parent);
		~PlaylistView () {}

		void setModel (QAbstractItemModel *model);

	public slots:
		void contextMenuEvent (QContextMenuEvent *e);

		void handleStatus (const Xmms::Playback::Status st);
		void invertSelection (void);
		void settingsSaved (void);
		void setPixmaps (Skin *skin);

	protected:
		void mouseDoubleClickEvent (QMouseEvent *event);

	private:
		Xmms::Playback::Status m_status;
		QFont *m_font;
		QFontMetrics *m_fontmetrics;
		QColor m_color_active;
		QColor m_color_selected;
		QColor m_color_normal;
		QColor m_color_normal_bg;
};


#endif

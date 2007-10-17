// TODO might not need those two
#include <xmmsclient/xmmsclient++.h>
#include "XMMSHandler.h"

#include "PlaylistView.h"
#include "playlistmodel.h"
#include "Playlist.h"

#include <QColor>
#include <QMenu>
#include <QPaintEvent>
#include <QPalette>
#include <QSettings>

#include <QSizePolicy>

/*
 * PlaylistDelegate
 */
PlaylistDelegate::PlaylistDelegate (QObject *parent) :
                  QAbstractItemDelegate (parent)
{
}

void
PlaylistDelegate::paint( QPainter *painter, const QStyleOptionViewItem& option,
                         const QModelIndex &index ) const
{
	painter->save ();

	/* Set background color */
	if ( option.state & QStyle::State_Selected ) {
		qDrawPlainRect (painter, option.rect, QColor("#FFFFFF"), 0,
		                &option.palette.brush (QPalette::Highlight));
	}

	/* Set forground color */
	if ( index.data (PlaylistModel::CurrentEntryRole).toBool () ) {
		painter->setPen (option.palette.brush (QPalette::BrightText).color ());
	} else {
		painter->setPen (option.palette.brush (QPalette::Text).color ());
	}

	/* generate string */
	//TODO Add album and playtime
	QVariant tmp;
	QString s = QString ("%1. ").arg (index.row () + 1);
	tmp = index.data ();
	if (tmp.isValid ())
		s.append (tmp.toString ()).append (" - ");
	QModelIndex m = index.sibling (index.row (), 1);
	tmp = m.data ();
	if (tmp.isValid ())
		s.append (tmp.toString ());
	s = option.fontMetrics.elidedText(s, Qt::ElideRight, option.rect.width());

	painter->drawText (option.rect, Qt::AlignVCenter, s);
	painter->restore ();
}

QSize
PlaylistDelegate::sizeHint ( const QStyleOptionViewItem &option,
                             const QModelIndex &index ) const
{
	/* For QListModel, width must be > 0, but is otherwise  */
	return QSize (1, option.font.pixelSize () +3);
}


/*
 * PlaylistView
 */
PlaylistView::PlaylistView (QWidget *parent) : QListView (parent)
{
	QSettings s;
	Skin *skin = Skin::getInstance ();

	if (!s.contains ("playlist/fontsize"))
		s.setValue ("playlist/fontsize", 10);

	setAttribute (Qt::WA_NoBackground);
	// TODO make drag and drop work
	//setDragEnabled(true);
	//setAcceptDrops(true);
	//setDropIndicatorShown (true);
	// end DragandDrop
	setFrameStyle(QFrame::NoFrame);
	setFocusPolicy (Qt::StrongFocus);
	setSelectionMode (QAbstractItemView::ExtendedSelection);
	setUniformItemSizes(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	// TODO make sure delegate gets deleted
	setItemDelegate (new PlaylistDelegate (this));

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	m_font = NULL;
	m_fontmetrics = NULL;

	XMMSHandler &xmmsh = XMMSHandler::getInstance ();

	connect (&xmmsh, SIGNAL(settingsSaved()),
	         this, SLOT(settingsSaved()));

	connect (&xmmsh, SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(handleStatus(Xmms::Playback::Status)));
}

void
PlaylistView::invertSelection () {
	QItemSelection selection = QItemSelection (model ()->index (0, 0),
	                           model ()->index (model ()->rowCount ()-1, 0));
	selectionModel ()->select (selection, QItemSelectionModel::Toggle |
	                                      QItemSelectionModel::Columns);
	selectionModel()->setCurrentIndex(model ()->index (0, 0),
	                                  QItemSelectionModel::NoUpdate);
}

void
PlaylistView::cropSelected () {
	invertSelection ();
	removeSelected ();
	selectAll ();
}

void
PlaylistView::removeSelected () {
	qobject_cast<PlaylistModel *> (model ())->removeRows (selectedIndexes ());
}

void
PlaylistView::setModel (QAbstractItemModel *model) {
	QListView::setModel (model);
	setModelColumn(0);
	updateGeometry();
}

void
PlaylistView::contextMenuEvent (QContextMenuEvent *e)
{
	QMenu qm(this);

	QAction *a;

	a = new QAction (tr ("Show file info"), this);
	a->setShortcut (tr ("Ctrl+Enter"));
	// connect (a, SIGNAL (triggered ()), this, SLOT (showMlib ()));
	qm.addAction (a);
	qm.addSeparator ();

	a = new QAction (tr ("Add file"), this);
	a->setShortcut (tr ("Ctrl+F"));
	qm.addAction (a);

	a = new QAction (tr ("Remove selected"), this);
	connect (a, SIGNAL (triggered ()), this, SLOT (removeSelected ()));
	qm.addAction (a);

	qm.addSeparator ();

	a = new QAction (tr ("Medialib browser"), this);
	qm.addAction (a);

	e->accept ();
	qm.exec (e->globalPos ());
}

void
PlaylistView::handleStatus (const Xmms::Playback::Status st)
{
	m_status = st;
}

void
PlaylistView::settingsSaved ()
{
	QSettings s;
	m_font->setPixelSize (s.value ("playlist/fontsize").toInt ());

	if (m_fontmetrics) {
		delete m_fontmetrics;
	}
	m_fontmetrics = new QFontMetrics (*m_font);
	update ();
}

void
PlaylistView::setPixmaps (Skin *skin)
{
	QSettings s;
	QPalette pal;
	QColor c;
	c.setNamedColor (skin->getPLeditValue ("normalbg"));
	QBrush b (c);
	pal.setBrush (QPalette::Window, b);
	setPalette (pal);

	if (m_font) {
		delete m_font;
	}
	m_font = new QFont (skin->getPLeditValue ("font"));
	m_font->setPixelSize (s.value ("playlist/fontsize").toInt ());

	if (m_fontmetrics) {
		delete m_fontmetrics;
	}
	m_fontmetrics = new QFontMetrics (*m_font);

	m_color_active.setNamedColor (skin->getPLeditValue ("current"));
	m_color_selected.setNamedColor (skin->getPLeditValue ("selectedbg"));
	m_color_normal.setNamedColor (skin->getPLeditValue ("normal"));
	m_color_normal_bg.setNamedColor (skin->getPLeditValue ("normalbg"));

	// remove later, in here for testing
	setFont(*m_font);
	pal.setColor (QPalette::Text, m_color_normal);
	pal.setColor (QPalette::BrightText, m_color_active);
	pal.setColor (QPalette::Base, m_color_normal_bg);
	pal.setColor (QPalette::Highlight, m_color_selected);
	setPalette (pal);

	update ();
}

void
PlaylistView::mouseDoubleClickEvent (QMouseEvent *event)
{
	QModelIndex index = indexAt(event->pos());
	if (!index.isValid()) {
		return;
	}

	XMMSHandler &xmmsh = XMMSHandler::getInstance ();
	xmmsh.requestTrackChange (index.row());
	if (m_status == XMMS_PLAYBACK_STATUS_STOP ||
	    m_status == XMMS_PLAYBACK_STATUS_PAUSE) {
			xmmsh.play ();
	}
}

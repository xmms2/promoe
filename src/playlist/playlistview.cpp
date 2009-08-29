/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "xclient.h"
#include "xplayback.h"

#include "application.h"
#include "playlistview.h"
#include "playlistmodel.h"
#include "playlistwidget.h"
#include "Skin.h"
#include "entryinfo.h"

#include <QColor>
#include <QMenu>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QSettings>
#include <QFontMetrics>

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
	QModelIndex m;
	QRect r = option.rect;
	QString s;
	// Get playtime and if it exists, draw it
	m = index.sibling (index.row (), 2);
	tmp = m.data ();
	if (tmp.isValid ()) {
		int seconds = tmp.toInt () / 1000;
		s = QString ("%1:%2").arg (seconds / 60, 2)
		                     .arg (seconds % 60, 2, 10, QLatin1Char ('0'));
		painter->drawText (r, Qt::AlignVCenter | Qt::AlignRight, s);
		// now adjust der paintrectangle for the remaining text
		r.setWidth (r.width () - option.fontMetrics.width (s));
	}

	// now build String for Artis Title and Position
	s = QString ("%1. ").arg (index.row () + 1);
	tmp = index.data ();
	if (tmp.isValid ())
		s.append (tmp.toString ()).append (" - ");
	m = index.sibling (index.row (), 1);
	tmp = m.data ();
	if (tmp.isValid ())
		s.append (tmp.toString ());
	s = option.fontMetrics.elidedText(s, Qt::ElideRight, r.width());

	painter->drawText (r, Qt::AlignVCenter, s);
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

	// Background is painted by PlaylistWidget
	setAttribute (Qt::WA_NoBackground);
	setFrameStyle(QFrame::NoFrame);
	setFocusPolicy (Qt::StrongFocus);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setSelectionMode (QAbstractItemView::ExtendedSelection);
	setUniformItemSizes(true);
	setDragEnabled(true);
	setAcceptDrops(true);

	// TODO make sure delegate gets deleted
	setItemDelegate (new PlaylistDelegate (this));

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	m_font = NULL;
	m_fontmetrics = NULL;

	const XClient *client = App->client ();

	connect (App, SIGNAL (settingsChanged ()),
	         this, SLOT (settingsChanged ()));

	connect (client->xplayback (), SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(handleStatus(Xmms::Playback::Status)));
}

void
PlaylistView::selectionChanged (const QItemSelection &selected,
                                const QItemSelection &deselected)
{
	/* selectedIndexes () is used here because selected only contains newly
	 * seleted indexes and will give a wrong result if pressing CONTROL to
	 * modify a selection */
	uint32_t playtime = qobject_cast<PlaylistModel *> (model ())->getPlaytimeForSelection (selectedIndexes ());
	emit selectionPlaytimeChanged (playtime);

	QListView::selectionChanged (selected, deselected);
}

void
PlaylistView::invertSelection ()
{
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
PlaylistView::setModel (PlaylistModel *plmodel) {
	if (model ()) disconnect (model (), 0, this, SLOT (currentPosChanged (QModelIndex)));
	QListView::setModel (plmodel);
	setModelColumn(0);
	updateGeometry();
	connect (plmodel, SIGNAL (currentPosChanged (QModelIndex)),
	         this, SLOT (currentPosChanged (QModelIndex)));
}

void
PlaylistView::currentPosChanged (QModelIndex index) {
	QSettings s;
	if (s.value ("playlist/scrolltocurrent", true).toBool ()) {
		scrollTo (index);
	}
}

void
PlaylistView::contextMenuEvent (QContextMenuEvent *e)
{
	QMenu qm(this);

	QAction *a;

	a = new QAction (tr ("Show file info"), this);
	a->setShortcut (tr ("Ctrl+Enter"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showEntryInfo ()));
	qm.addAction (a);
	qm.addSeparator ();

	a = new QAction (tr ("Add file"), this);
	a->setShortcut (tr ("Ctrl+F"));
	connect (a, SIGNAL (triggered ()),
	         qobject_cast<PlaylistWidget *> (parent ()), SLOT (menuAddFile ()));
	qm.addAction (a);

	a = new QAction (tr ("Remove selected"), this);
	connect (a, SIGNAL (triggered ()), this, SLOT (removeSelected ()));
	qm.addAction (a);

	qm.addSeparator ();

	a= new QAction (tr ("Choose Playlist"), this);
	connect (a, SIGNAL (triggered ()), qobject_cast<PlaylistWidget *>(parent ()), SLOT (openPlaylistChooser ()));
	qm.addAction (a);

	a = new QAction (tr ("Medialib browser"), this);
	// connect (a, SIGNAL (triggered ()), this, SLOT (showMlib ()));
	a->setEnabled(false); //FIXME: Disabled for now
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
PlaylistView::settingsChanged ()
{
	QSettings s;
	m_font->setPixelSize (s.value ("playlist/fontsize").toInt ());

	if (m_fontmetrics) {
		delete m_fontmetrics;
	}
	m_fontmetrics = new QFontMetrics (*m_font);
	update ();
	// Scroll to current pos, if enabled
	currentPosChanged (qobject_cast<PlaylistModel *>(model())->currentPos ());
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

	const XClient *client = App->client ();
	client->xplayback ()->setPos (index.row());
	if (m_status == XMMS_PLAYBACK_STATUS_STOP ||
	    m_status == XMMS_PLAYBACK_STATUS_PAUSE) {
			client->xplayback ()->play ();
	}
}

void
PlaylistView::showEntryInfo (void)
{
	const XClient *client = App->client ();
	QModelIndex current = selectionModel ()->currentIndex ();
	if (current.isValid ()) {
		uint32_t id = model ()->data (current, PlaylistModel::MedialibIdRole)
		                        .toUInt ();
		// If no infodialog exists, create one, else set the selected Item as
		// displayed item
		if (!m_entry_info) {
			m_entry_info = new EntryInfo (this, client->cache (), id);
		} else {
			m_entry_info->raise ();
			m_entry_info->setId (id);
		}
		m_entry_info->show ();
	}
}

#include "playlistview.moc"

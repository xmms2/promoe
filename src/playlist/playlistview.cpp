/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
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
#include "entryinfo.h"
#include "playlistview.h"
#include "playlistmodel.h"
#include "playlistwidget.h"
#include "skin.h"
#include "skinmanager.h"

#include <QColor>
#include <QMenu>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QSettings>
#include <QFontMetrics>

#include <QSizePolicy>

#include <qdrawutil.h>

// L=left, R=right, N=normal (not active), A=active, S=selected
#define DFMT_LN    "%#2ca4c7%@. <r>%r - </r>%#------%t %#444444 %c"
#define DFMT_RN    "<l>%#12576b<n>%n:</n>%l %#------</l>%d"
#define DFMT_LNS   "%#2ca4c7%@. <r>%r - </r>%#------%t %#444444 %c"
#define DFMT_RNS   "<l>%#003342<n>%n:</n>%l %#------</l>%d"
#define DFMT_LA    "%@. <r>%r - </r>%t %#555555 %c"
#define DFMT_RA    "<l><n>%n:</n>%l </l>%d"
#define DFMT_LAS   "%@. <r>%r - </r>%t %#555555 %c"
#define DFMT_RAS   "<l><n>%n:</n>%l </l>%d"

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
	QSettings s;

	painter->save ();

	/* Set background color */
	if ( option.state & QStyle::State_Selected ) {
		QString selColour = s.value ("playlist/format_clr_sel_bg", QString ()).toString ();
		QBrush b;
		if ( selColour.isEmpty () ) {
			b = option.palette.brush (QPalette::Highlight);
		} else {
			b = QBrush (QColor (selColour));
		}
		qDrawPlainRect (painter, option.rect, QColor ("#FFFFFF"), 0, &b);
	}

	/* Set forground color */
	bool isCurrent = index.data (PlaylistModel::CurrentEntryRole).toBool ();

	QRect r = option.rect;

	// Extract all the data we need
	QMap<char, QString> placeholderValues;
	QVariant tmp;

	placeholderValues['@'] = QString::number (index.row () + 1, 10);

	tmp = index.sibling (index.row (), 2).data ();
	if ( tmp.isValid () ) {
		int seconds = tmp.toInt () / 1000;
		placeholderValues['d'] = QString ("%1:%2").arg (seconds / 60)
		                                          .arg (seconds % 60, 2, 10,
		                                                QLatin1Char ('0'));
	} else {
		// Normal dashes ("-:--") don't line up with the digits ("0:00") in the
		// rows above and below, so use a wider Unicode dash.
		placeholderValues['d'] = QString::fromUtf8 ("\u2013:\u2013\u2013");
	}

	// A(r)tist
	tmp = index.data ();
	placeholderValues['r'] = tmp.isValid () ? tmp.toString () : QString ();

	// Song (t)itle
	tmp = index.sibling (index.row (), 1).data ();
	placeholderValues['t'] = tmp.isValid () ? tmp.toString () : QString ();

	// A(l)bum
	tmp = index.sibling (index.row (), 3).data ();
	placeholderValues['l'] = tmp.isValid () ? tmp.toString () : QString ();

	// Track (n)umber
	tmp = index.sibling (index.row (), 4).data ();
	placeholderValues['n'] = tmp.isValid () ? tmp.toString () : QString ();

	// Play (c)ount
	tmp = index.sibling (index.row (), 5).data ();
	placeholderValues['c'] = tmp.isValid () ? tmp.toString () : QString ();

	QVector<QString> format;

	// We always start with a colour specifier ("%#") and the colour "------"
	// or "++++++" so that we can rely on the string list always starting with
	// a colour code, even if the user hasn't specified one.  This simplifies
	// handling the colours later.
	if ( isCurrent ) {
		if ( option.state & QStyle::State_Selected ) {
			format.append (
				QString ("%#++++++%1").arg (
					s.value ("playlist/format_r_active_selected", DFMT_RAS).toString ()
				)
			);
			format.append (
				QString ("%#++++++%1").arg (
					s.value ("playlist/format_l_active_selected", DFMT_LAS).toString ()
				)
			);
		} else {
			format.append (
				QString ("%#++++++%1").arg (
					s.value ("playlist/format_r_active", DFMT_RA).toString ()
				)
			);
			format.append (
				QString ("%#++++++%1").arg (
					s.value ("playlist/format_l_active", DFMT_LA).toString ()
				)
			);
		}
	} else {
		if ( option.state & QStyle::State_Selected ) {
			format.append (
				QString ("%#------%1").arg (
					s.value ("playlist/format_r_normal_selected", DFMT_RNS).toString ()
				)
			);
			format.append (
				QString ("%#------%1").arg (
					s.value ("playlist/format_l_normal_selected", DFMT_LNS).toString ()
				)
			);
		} else {
			format.append (
				QString ("%#------%1").arg (
					s.value ("playlist/format_r_normal", DFMT_RN).toString ()
				)
			);
			format.append (
				QString ("%#------%1").arg (
					s.value ("playlist/format_l_normal", DFMT_LN).toString ()
				)
			);
		}
	}

	// See if we need to shorten any fields to get the whole song title visible

	QMap<char, bool> shortened;
	shortened['r'] = false;
	shortened['l'] = false;

	QVector<QStringList> output;
	QVector<QString> previewText;

	// Do this a few times until we get it right.  Must be one more than the
	// number of fields we abbreviate, so the text gets regenerated after the
	// last field has been shortened.  This is also important because if, for
	// example the space is so short the album title is made completely blank,
	// then a <l></l> conditional will activate and possibly hide some
	// additional information like the track number and separating symbol.
	for ( int retry = 0; retry < 3; retry++ ) // 3 = artist, album, +1
	{
		output.clear ();
		previewText.clear ();

		for ( QVector<QString>::const_iterator f = format.constBegin ();
		      f != format.constEnd ();
		      f++ )
		{
			QString fmt = *f; // copy to preserve original

			// Remove all the conditional sections based on whether data is present
			for ( QMap<char, QString>::iterator p = placeholderValues.begin ();
			      p != placeholderValues.end ();
			      p++ )
			{
				const char &key = p.key ();
				const QString &value = p.value ();

				// This regex will match the section that should be removed if that
				// tag is empty.  If it's empty, the whole thing is removed tags
				// and all, but if the placeholder has a value then only the start
				// and end tags are removed.
				QRegExp reg (QString ("<%1>(.*)</%1>").arg (key));
				fmt.replace (reg, value.isEmpty () ? "" : "\\1");

				// Put the final value in too while we're here
				fmt.replace (QString ('%') + key, value);
			}

			QStringList parts = fmt.split ("%#", QString::SkipEmptyParts);
			output.append (parts);

			QString pt;
			for ( QStringList::const_iterator s = parts.begin ();
			      s != parts.end ();
			      s++ )
			{
				pt.append (s->mid (6));
			}
			previewText.append (pt);
		}

		// Measure the text to see if it will fit
		QRect rcLeft = option.fontMetrics.boundingRect (r, Qt::AlignVCenter, previewText[1]);
		QRect rcRight = option.fontMetrics.boundingRect (r, Qt::AlignVCenter | Qt::AlignRight, previewText[0]);
		int widthLeft = rcLeft.width ();
		int widthRight = rcRight.width ();
		if ( widthLeft + widthRight > r.width () ) {
			// The left text will run into the right, so crop something
			unsigned int excess = 4 + widthLeft + widthRight - r.width ();

			QString *target = nullptr;
			if ( !shortened['l'] ) {
				// Shorten the album first
				shortened['l'] = true;
				target = &placeholderValues['l'];

			} else if ( !shortened['r'] ) {
				// Then the artist
				shortened['r'] = true;
				target = &placeholderValues['r'];
			}

			if ( target ) {
				if ( !target->isEmpty () ) {
					QRect rcField = option.fontMetrics.boundingRect (r, Qt::AlignVCenter, *target);
					int maxFieldWidth = rcField.width () - excess;
					if ( maxFieldWidth < 0) {
						// No room at all for this field
						*target = QString ();
					} else {
						*target = option.fontMetrics.elidedText (*target,
						                                         Qt::ElideRight,
						                                         maxFieldWidth);
					}
				}
			} else {
				// No need to retry
				break;
			}
		}

	} // for (retry until we can get the song title to fit)

	// Now draw the left and right text for this song

	const QStringList &slRight = output[0];
	const QStringList &slLeft = output[1];

	// Do the left-aligned text first
	for ( int i = 0; i < slLeft.size (); i++ ) {
		QString colourCode = QString ("#").append (slLeft[i].mid (0, 6));
		QString content = slLeft[i].mid (6);

		QColor c;
		if ( colourCode.compare ("#------") == 0) {
			c = option.palette.brush (QPalette::Text).color ();
		} else if ( colourCode.compare ("#++++++") == 0) {
			c = option.palette.brush (QPalette::BrightText).color ();
		} else {
			c = QColor (colourCode);
		}
		painter->setPen (c);

		QRect rcResult; // need a fresh one for each drawText()
		painter->drawText (r, Qt::AlignVCenter, content, &rcResult);
		// Move the drawing point across by as much as we just drew
		r.setLeft (r.left () + rcResult.width ());
	}

	// Followed by the right-aligned text
	for ( int i = slRight.size () - 1; i >= 0; i-- ) {
		QString colourCode = QString ("#").append (slRight[i].mid (0, 6));
		QString content = slRight[i].mid (6);

		QColor c;
		if ( colourCode.compare ("#------") == 0) {
			c = option.palette.brush (QPalette::Text).color ();
		} else if ( colourCode.compare ("#++++++") == 0) {
			c = option.palette.brush (QPalette::BrightText).color ();
		} else {
			c = QColor (colourCode);
		}
		painter->setPen (c);

		QRect rcResult; // need a fresh one for each drawText()
		painter->drawText (r, Qt::AlignVCenter | Qt::AlignRight, content, &rcResult);
		// Move the drawing point back by as much as we just drew
		r.setWidth (r.width () - rcResult.width ());
	}


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
	Skin *skin = SkinManager::instance ()->activeSkin ();

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

	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
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
		QRect rc = visualRect (index);
		ScrollHint sh = EnsureVisible;
		int push = height() * 0.1;
		if ((rc.y() < push) || (rc.y() > height() - push)) {
			// Outside the viewable area or close to the edge, scroll to middle
			sh = PositionAtCenter;
		}
		scrollTo (index, sh);
	}
}

void
PlaylistView::contextMenuEvent (QContextMenuEvent *e)
{
	QSettings s;
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

	a = new QAction (tr ("Show tooltips"), this);
	connect (a, SIGNAL (triggered ()), this, SLOT (configureTooltips ()));
	a->setCheckable(true);
	a->setChecked (s.value ("playlist/showtooltips", true).toBool ());
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
		App->showEntryInfo (id);
	}
}

void
PlaylistView::configureTooltips (void)
{
	QSettings s;
	s.setValue ("playlist/showtooltips", !s.value("playlist/showtooltips", true).toBool ());
}

#include "playlistview.moc"

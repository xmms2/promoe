#include <xmmsclient/xmmsclient++.h>
#include "XMMSHandler.h"

#include "PlaylistList.h"
#include "Playlist.h"

#include <QStyleOptionHeader>
#include <QPaintEvent>
#include <QSettings>
#include <QCursor>
#include <QDrag>
#include <QMenu>

PlaylistItem::PlaylistItem (PlaylistList *pl, uint id)
{
	m_pl = pl;
	m_id = id;
	m_req = false;
	m_duration = QString ("00:00");
	pl->addItem (this);
}

PlaylistItem::PlaylistItem (PlaylistList *pl, uint id, uint pos)
{
	m_pl = pl;
	m_id = id;
	m_req = false;
	m_duration = QString ("00:00");
	pl->addItem (this, pos);
}

QString
PlaylistItem::text (void)
{

	if (m_text.count() < 1) {

		if (!m_req) {
			XMMSHandler::getInstance ().requestMediainfo (m_id);
			m_req = true;
		}

		QString q;
		q.setNum (m_id);
		return q;
	} else {
		m_req = false;
		return m_text;
	}
}

PlaylistList::PlaylistList (QWidget *parent) : QWidget (parent)
{
	QSettings s;
	XMMSHandler &xmmsh = XMMSHandler::getInstance ();
	Skin *skin = Skin::getInstance ();

	if (!s.contains("playlist/fontsize"))
		s.setValue ("playlist/fontsize", 10);

	setAttribute (Qt::WA_NoBackground);
	setFocusPolicy (Qt::StrongFocus);

	setAcceptDrops (true);

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	m_font = NULL;
	m_fontmetrics = NULL;
	m_items = new QList<PlaylistItem *>;
	m_selected = new QList<uint>;
	m_itemmap = new QHash<uint, PlaylistItem *>;
	m_offset = 0;
	m_status = XMMS_PLAYBACK_STATUS_STOP;
	m_bar = -2;

	connect (&xmmsh, SIGNAL(playlistList(const QList<uint> &)),
	         this, SLOT(playlistList(const QList<uint> &)));

	connect (&xmmsh, SIGNAL(currentID(uint)),
	         this, SLOT(currentID(uint)));

	connect (&xmmsh, SIGNAL(mediainfoChanged(uint, const QHash<QString, QString> &)), 
	         this, SLOT(mediainfoChanged(uint, const QHash<QString, QString> &)));

	connect (&xmmsh, SIGNAL(playlistChanged(const QHash<QString, QString> &)),
	         this, SLOT(playlistChanged(const QHash<QString, QString> &)));

	connect (&xmmsh, SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(setStatus(Xmms::Playback::Status)));

	connect (&xmmsh, SIGNAL(settingsSaved()),
	         this, SLOT(settingsSaved()));
}

void
PlaylistList::settingsSaved ()
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
PlaylistList::setStatus (Xmms::Playback::Status s)
{
	m_status = s;
}

void
PlaylistList::playlistChanged (const QHash<QString,QString> &h)
{
	int signal = h.value("type").toUInt();
	XMMSHandler &xmmsh = XMMSHandler::getInstance ();
	switch (signal) {
		case XMMS_PLAYLIST_CHANGED_ADD:
			{
				uint id = h.value("id").toUInt();
				if (m_itemmap->contains (id)) {
					addItem (m_itemmap->value (id));
				} else {
					new PlaylistItem (this, id);
				}
			}
			break;
		case XMMS_PLAYLIST_CHANGED_INSERT:
			{
				uint id = h.value("id").toUInt ();
				uint pos = h.value("position").toUInt ();

				if (m_itemmap->contains (id)) {
					addItem (m_itemmap->value (id));
				} else {
					new PlaylistItem (this, id, pos);
				}

			}
			break;
		case XMMS_PLAYLIST_CHANGED_REMOVE:
			{
				int pos = h.value("position").toUInt();
				PlaylistItem *i = m_items->value (pos);

				if (!i) {
					return;
				}

				m_items->removeAt (pos);
				if (!m_items->contains (i)) {
					m_itemmap->remove (i->getID ());
					delete i;
				} 

				if (m_active) {
					if (m_active > pos) {
						m_active --;
					} else if (m_active == pos) {
						m_active = 0;
					}
				}

				doResize ();

			}
			break;
		case XMMS_PLAYLIST_CHANGED_MOVE:
			break;
		case XMMS_PLAYLIST_CHANGED_CLEAR:
		case XMMS_PLAYLIST_CHANGED_SHUFFLE:
		case XMMS_PLAYLIST_CHANGED_SORT:
			{
				while (!m_items->isEmpty()) {
					PlaylistItem *item = m_items->takeFirst();
					m_items->removeAll (item);
					delete item;
				}

				m_itemmap->clear ();

				if (signal != XMMS_PLAYLIST_CHANGED_CLEAR) {
					xmmsh.requestPlaylistList ();
				} else {
					doResize ();
				}
			}
			break;
	}

	update ();
}

void
PlaylistList::currentID (uint id)
{
	PlaylistItem *i = m_itemmap->value (id);
	if (!i) {
		m_active = -1;
		return;
	}
	
	m_active = m_items->indexOf (i);

	update ();
}

void
PlaylistList::mediainfoChanged (uint id, const QHash<QString, QString> &h)
{
	PlaylistItem *i = m_itemmap->value (id);
	if (i) {
		QString n;
		if (h.contains ("artist") && h.contains ("album") && h.contains ("title")) {
			n = h.value("artist") + " - " + h.value("album") + " - " + h.value("title");
		} else {
			if (h.contains ("channel")) {
				n = h.value("channel") + " - " + h.value("title");
			} else {
				QString t = h.value("url");
				n = t.section ("/", -1);
			}
		}
		i->setText (n);
		if (h.contains ("duration")) {
			int d = h.value("duration").toInt();
			QString dur;
			dur.sprintf ("%02d:%02d", d/60000, (d/1000)%60);
			i->setDuration (dur);
		}
	}
		
	update ();
}

void
PlaylistList::playlistList (const QList<uint> &l)
{
	for (int i = 0; i < l.count(); i++) {
		if (m_itemmap->contains (l.value(i))) {
			addItem (m_itemmap->value (l.value (i)));
		} else {
			new PlaylistItem (this, l.value(i));
		}
	}

	update ();
}

void
PlaylistList::mouseDoubleClickEvent (QMouseEvent *event)
{
	if (m_items->count() < 1 || m_selected->count() < 1) {
		return;
	}

	PlaylistItem *it = m_items->value (m_selected->first());
	if (!it) {
		return;
	}

	XMMSHandler &xmmsh = XMMSHandler::getInstance ();
	xmmsh.requestTrackChange (m_items->indexOf(it));
	if (m_status == XMMS_PLAYBACK_STATUS_STOP ||
		m_status == XMMS_PLAYBACK_STATUS_PAUSE)
	{
		xmmsh.play ();
	}
}

QPixmap 
PlaylistList::generatePixmap (int i)
{
	QStyleOptionHeader opt;
	QString t (m_items->value(i)->text());

	QPixmap p (m_fontmetrics->width(t), getFontH());

	QPainter paint;
	paint.begin (&p);
	paint.setFont (*m_font);
	paint.setPen (QPen (m_color_normal));
	paint.fillRect (p.rect(), QBrush (m_color_normal_bg));
	paint.drawText (p.rect(), Qt::TextSingleLine, t);
	paint.end ();

	return p;
}

void
PlaylistList::showMenu (void)
{
	QMenu qm(this);

	QAction *a;

	a = new QAction (tr ("Show file info"), this);
	a->setShortcut (tr ("Ctrl+Enter"));
	/*
	connect (a, SIGNAL (triggered ()), this, SLOT (showMlib ()));
	*/
	qm.addAction (a);
	qm.addSeparator ();

	a = new QAction (tr ("Add file"), this);
	a->setShortcut (tr ("Ctrl+F"));
	qm.addAction (a);

	a = new QAction (tr ("Remove selected"), this);
	qm.addAction (a);

	qm.addSeparator ();

	a = new QAction (tr ("Medialib browser"), this);
	qm.addAction (a);

	qm.exec (QCursor::pos ());
}

void 
PlaylistList::mousePressEvent (QMouseEvent *event)
{
	if (m_items->count() < 1) {
		return;
	}

	int i = ((event->pos().y()+m_offset) / getFontH());

	if (i < 0) {
		i = 0;
	}

	if (i > (m_items->count () - 1)) {
		return;
	}

	if (event->button () == Qt::LeftButton) {
		if (event->modifiers() & Qt::ShiftModifier) {
			if (m_selected->count () > 0) {
				int o = m_selected->last ();
				if (o < i) {
					for (int y = o+1; y <= i; y++) {
						m_selected->append (y);
					}
				} else {
					for (int y = i; y < o; y++) {
						m_selected->append (y);
					}
				}
			} else {
				m_selected->append (i);
			}
		} else if (event->modifiers () & Qt::ControlModifier) {
			if (m_selected->contains (i)) {
				m_selected->removeAll (i);
			} else {
				m_selected->append (i);
			}
		} else {
			if (m_selected->contains (i)) {
				m_selected->clear();
			} else {
				m_selected->clear();
				m_selected->append(i);
			}

			m_dragstart = event->pos ();
		}
	} else if (event->button () == Qt::RightButton) {
		showMenu ();
	}

	update ();
}

void
PlaylistList::mouseMoveEvent (QMouseEvent *event)
{

	if (!(event->buttons() & Qt::LeftButton))
		return;
	if ((event->pos() - m_dragstart).manhattanLength() < QApplication::startDragDistance())
		return;

	if (m_selected->count() > 0) {
		int i = m_selected->last ();

		m_drag = new QDrag (this);

		m_md = new QMimeData();
		QByteArray encodedData;

		QDataStream stream (&encodedData, QIODevice::WriteOnly);
		stream << QString::number (m_selected->last ());

		m_md->setData("application/playlist.move", encodedData);

		m_drag->setMimeData (m_md);

		m_drag_id = m_items->value (i)->getID ();
		m_pos = i;

		QPixmap p = generatePixmap (i);

		m_drag->setPixmap (p);
		m_items->removeAt (i);
		m_selected->clear ();

		m_drag->start ();
	}

}

void
PlaylistList::dragEnterEvent (QDragEnterEvent *event)
{

	if (event->mimeData()->hasFormat("application/mlib.album") ||
		event->mimeData()->hasFormat("application/mlib.artist") ||
		event->mimeData()->hasFormat("application/mlib.song") ||
		event->mimeData()->hasFormat("application/playlist.move"))
		event->acceptProposedAction();
}

void
PlaylistList::dragMoveEvent (QDragMoveEvent *event)
{
	int i = ((event->pos().y()+m_offset) / getFontH());
	if (event->pos().y() < getFontH() / 2) {
		m_bar = -1;
	} else if (i >= m_items->count ()) {
		m_bar = m_items->count ()-1;
	} else {
		m_bar = i;
	}
	update ();
}

void
PlaylistList::dragLeaveEvent (QDragLeaveEvent *event)
{
	m_bar = -2;
	update ();
}

void
PlaylistList::dropEvent (QDropEvent *event)
{
	XMMSHandler &xmmsh = XMMSHandler::getInstance ();

	if (event->mimeData()->hasFormat("application/playlist.move")) {
		if (m_bar == -2) {
			m_items->insert (m_pos, m_itemmap->value (m_drag_id));
		} else {
			m_items->insert (m_bar + 1, m_itemmap->value (m_drag_id));
			xmmsh.playlistMove (m_pos, m_bar + 1);
		}
		m_selected->append (m_drag_id);

		event->acceptProposedAction ();
	}
	/*
	} else if (event->mimeData()->hasFormat("application/mlib.album")) {
		const QMimeData *md = event->mimeData ();
		QByteArray encodedData = md->data("application/mlib.album");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

		QString artist;
		QString album;
		stream >> artist;
		stream >> album;

		QString query;
		if (artist == "Various Artists") {
			query.sprintf ("select m1.id as id, ifnull(m3.value,-1) as tracknr from Media m1 join Media m2 on m1.id = m2.id and m2.key='compilation' left join Media m3 on m1.id = m3.id and m3.key='tracknr' where m1.key='album' and m1.value='%s' and m2.value=1 order by tracknr", album.toUtf8 ().data ());
		} else {
			query.sprintf ("select m1.id as id, ifnull(m3.value,-1) as tracknr from Media m1 join Media m2 on m1.id = m2.id and m2.key='album' left join Media m3 on m1.id = m3.id and m3.key='tracknr' where m1.key='artist' and m1.value='%s' and m2.value='%s' order by tracknr", artist.toUtf8 ().data (), album.toUtf8 ().data ());
		}

		xmmsh->medialibQueryAdd (query);

		event->acceptProposedAction ();
	} else if (event->mimeData()->hasFormat("application/mlib.artist")) {
		const QMimeData *md = event->mimeData ();
		QByteArray encodedData = md->data("application/mlib.artist");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

		QString artist;
		stream >> artist;

		QString query;
		if (artist == "Various Artists") {
			query.sprintf ("select m1.id as id, m2.value as album, ifnull(m3.value,-1) as tracknr from Media m1 left join Media m2 on m1.id = m2.id and m2.key='album' left join Media m3 on m1.id = m3.id and m3.key='tracknr' where m1.key='compilation' and m1.value=1 order by album, tracknr;");
		} else {
			query.sprintf ("select m1.id as id, m2.value as album, ifnull(m3.value,-1) as tracknr from Media m1 left join Media m2 on m1.id = m2.id and m2.key='album' left join Media m3 on m1.id = m3.id and m3.key='tracknr' where m1.key='artist' and m1.value='%s' order by album, tracknr", artist.toUtf8 ().data ());
		}

		xmmsh->medialibQueryAdd (query);
		event->acceptProposedAction ();
	} else if (event->mimeData()->hasFormat("application/mlib.song")) {
		const QMimeData *md = event->mimeData ();
		QByteArray encodedData = md->data("application/mlib.song");
		QDataStream stream(&encodedData, QIODevice::ReadOnly);

		QString artist;
		QString album;
		QString song;
		stream >> artist;
		stream >> album;
		stream >> song;

		QString query;
		query.sprintf ("select m1.id as id, ifnull(m1.value, '[unknown]') as artist, ifnull(m2.value,'[unknown]') as album, ifnull(m4.value,m5.value) as song from Media m1 left join Media m2 on m1.id = m2.id and m2.key='album' left join Media m4 on m1.id = m4.id and m4.key='title' left join Media m5 on m1.id = m5.id and m5.key='url' where m1.key='artist' and artist='%s' and album='%s' and song='%s'", artist.toUtf8 ().data (), album.toUtf8 ().data (), song.toUtf8 ().data ());

		xmmsh->medialibQueryAdd (query);

		event->acceptProposedAction ();
	}
	m_bar = -2;
	update ();
	*/
}

void
PlaylistList::keyPressEvent (QKeyEvent *event)
{

	QWidget *w = dynamic_cast<QWidget*>(parent());
	QSize s = w->size ();
	int lastitem = (m_offset + s.height()) / getFontH () - 1;
	if (lastitem > m_items->count())
		lastitem = m_items->count() - 1;
	int firstitem = m_offset / getFontH();

	switch (event->key ()) {
		case Qt::Key_Down:
			{
				int i = m_selected->last ();
				i ++;

				if (i > (m_items->count () - 1))
					i = m_items->count () - 1;

				m_selected->clear ();
				m_selected->append (i);

				if (i > lastitem)
					setOffset (m_offset += getFontH ());

				update ();
				emit sizeChanged (size());
			}
			break;
		case Qt::Key_Up:
			{
				int i = m_selected->last ();
				i --;
				if (i < 0)
					i = 0;
				m_selected->clear ();
				m_selected->append (i);
				if (i < firstitem)
					setOffset (m_offset -= getFontH ());

				update ();
				emit sizeChanged (size());
			}
			break;
		case Qt::Key_Backspace:
		case Qt::Key_Delete:
			{
				deleteFiles ();
			}
			break;
		case Qt::Key_A:
			{
				if (event->modifiers() == Qt::ControlModifier) {
					m_selected->clear ();
					for (int i = 0; i < m_items->count (); i ++) {
						m_selected->append (i);
					}
					update ();
				}
			}
			break;
	}

}

void
PlaylistList::deleteFiles ()
{
	XMMSHandler &xmmsh = XMMSHandler::getInstance ();

	//Sort list and remove in reverse order
	qSort (*m_selected);
	for (int i = (m_selected->count () - 1); i >= 0; i --) {
		xmmsh.playlistRemove (m_selected->value (i));
	}
	m_selected->clear ();
}

void
PlaylistList::paintEvent (QPaintEvent *event)
{
	int i;
	QPainter paint;
	paint.begin (this);
	paint.setFont (*m_font);
	/*
	paint.setClipping (false);
	*/
	paint.setPen (QPen (m_color_normal));

	int cy = event->rect().y () + m_offset;
	int ch = event->rect().height();
	int sitem = cy / getFontH();
	int eitem = (cy + ch) / getFontH();
	int mod = m_offset - (getFontH() * sitem);

	if (eitem > m_items->count())
		eitem = m_items->count();

	QString q;
	QRect fullLine; 
	QRect textLine;
	QRect numLine;

	for (i = sitem; i < eitem; i++) {
		PlaylistItem *item = m_items->value (i);

		int tw = m_fontmetrics->width(item->duration ())+2;

		fullLine.setRect(0, (getFontH()*(i-sitem)) - mod, 
						 size().width(), getFontH());
		textLine.setRect(0, (getFontH()*(i-sitem)) - mod, 
						 size().width()-tw-4, getFontH());
		numLine.setRect(size().width()-tw, (getFontH()*(i-sitem)) - mod, 
						 tw, getFontH());

		q = QString::number (i + 1) + ". " + item->text ();

		if (m_selected->contains (i)) {
			paint.fillRect (fullLine, QBrush (m_color_selected));
		} else {
			paint.eraseRect (fullLine);
		}

		if (m_active == i) {
			paint.setPen (QPen (m_color_active));
			paint.drawText (textLine, Qt::TextSingleLine, q);
			paint.drawText (numLine, item->duration ());
			paint.setPen (QPen (m_color_normal));
		} else {
			paint.drawText (textLine, Qt::TextSingleLine, q);
			paint.drawText (numLine, item->duration ());
		}

		if (m_bar == -1) {
			paint.save ();
			QPen pen (m_color_active);
			pen.setWidth (2);
			paint.setPen (pen);
			paint.drawLine (2, 0, size().width()-2, 0);
			paint.restore ();
		} else if (m_bar == i) {
			paint.save ();
			QPen pen (m_color_active);
			pen.setWidth (5);
			paint.setPen (pen);
			paint.drawLine (2, fullLine.y()+getFontH(), size().width()-2, 
							fullLine.y()+getFontH());
			paint.restore ();
		}

	}

	if ((getFontH()*(i-sitem) - mod) < size().height()) {
		paint.eraseRect (QRect (0, (getFontH()*(i-sitem) - mod), 
								size().width(), 
								size().height()-(getFontH()*(i-sitem) - mod)));
	}

	paint.end ();

}

void
PlaylistList::doResize (void)
{
	QWidget *w = dynamic_cast<QWidget*>(parent());
	QSize s = w->size ();

	if (m_items->count()*getFontH () > s.height ()) {
		resize (size().width(), m_items->count ()*getFontH ());
	} else {
		resize (size().width(), s.height());
		setOffset (0);
	}
	emit sizeChanged (size());
}

void
PlaylistList::addItem (PlaylistItem *i, uint pos)
{
	m_items->insert (pos, i);
	if (!m_itemmap->contains (i->getID())) {
		m_itemmap->insert (i->getID(), i);
	}
	doResize ();
}

void
PlaylistList::addItem (PlaylistItem *i)
{
	m_items->append (i);
	if (!m_itemmap->contains (i->getID())) {
		m_itemmap->insert (i->getID(), i);
	}
	doResize ();
}

int
PlaylistList::getFontH (void)
{
	if (!m_fontmetrics) {
		return 0;
	}
	return m_fontmetrics->height();
}

void
PlaylistList::setPixmaps (Skin *skin)
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

	update ();
}

void
PlaylistList::setSize (int width, int height)
{
	int nx, ny;
	nx = width;

	if (height > size().height()) {
		ny = height;
	} else {
		ny = size().height();
	}

	resize (nx, ny);
}



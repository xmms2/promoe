#include "XMMSHandler.h"

#include "PlaylistList.h"
#include "Playlist.h"

#include <QPaintEvent>

PlaylistItem::PlaylistItem (PlaylistList *pl, uint id)
{
	m_pl = pl;
	m_id = id;
	m_isactive = false;
	m_isselected = false;
	m_req = false;
	if (getSelected ()) {
		qDebug ("trasigt!");
	}
	pl->addItem (this);
}

QString
PlaylistItem::text (void)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	if (m_text.count() < 1) {

		if (!m_req) {
			xmmsh->requestMediainfo (m_id);
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
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	PlaylistWindow *pl = dynamic_cast<PlaylistWindow*>(window ());
	connect (pl->getSkin (), SIGNAL (skinChanged (Skin *)), this, SLOT (setPixmaps(Skin *)));
	m_font = NULL;
	m_fontmetrics = NULL;
	m_items = new QList<PlaylistItem *>;
	m_selected = new QList<uint>;
	m_itemmap = new QHash<uint, PlaylistItem *>;
	m_offset = 0;

	connect (xmmsh, SIGNAL(playlistList(QList<uint>)), this, SLOT(playlistList(QList<uint>)));
	connect (xmmsh, SIGNAL(currentID(uint)), this, SLOT(currentID(uint)));
	connect (xmmsh, SIGNAL(mediainfoChanged(uint, QHash<QString, QString>)), 
			 this, SLOT(mediainfoChanged(uint, QHash<QString, QString>)));
	connect (xmmsh, SIGNAL(playlistChanged(QHash<QString, QString>)),
			 this, SLOT(playlistChanged(QHash<QString, QString>)));
}

void
PlaylistList::playlistChanged (QHash<QString,QString> h)
{
	int signal = h.value("type").toUInt();
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();
	switch (signal) {
		case XMMS_PLAYLIST_CHANGED_ADD:
			new PlaylistItem (this, h.value("id").toUInt());
			break;
		case XMMS_PLAYLIST_CHANGED_INSERT:
			break;
		case XMMS_PLAYLIST_CHANGED_REMOVE:
			{
				int pos = h.value("position").toUInt();
				PlaylistItem *i = m_items->value (pos);
				if (!i) {
					qDebug ("no item in playlist?");
					return;
				}
				m_items->removeAt (pos);
				m_itemmap->remove (i->getID ());
				delete i;
			}
			break;
		case XMMS_PLAYLIST_CHANGED_MOVE:
			break;
		case XMMS_PLAYLIST_CHANGED_CLEAR:
		case XMMS_PLAYLIST_CHANGED_SHUFFLE:
		case XMMS_PLAYLIST_CHANGED_SORT:
			{
				m_itemmap->clear ();
				while (!m_items->isEmpty())
					delete m_items->takeFirst();

				xmmsh->requestPlaylistList ();
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
		return;
	}
	i->setActive (true);

	i = m_itemmap->value (m_active);
	if (!i) {
		update ();
		m_active = id;
		return;
	}
	i->setActive (false);

	m_active = id;
	update ();
}

void
PlaylistList::mediainfoChanged (uint id, QHash<QString, QString> h)
{
	PlaylistItem *i = m_itemmap->value (id);
	if (i) {
		QString n;
		if (h.contains ("artist") && h.contains ("album") && h.contains ("title")) {
			n = h.value("artist") + " - " + h.value("album") + " - " + h.value("title");
		} else {
			n = h.value("url");
		}
		i->setText (n);
	}

	update ();
}

void
PlaylistList::playlistList (QList<uint> l)
{
	for (int i = 0; i < l.count(); i++) {
		new PlaylistItem (this, l.value(i));
	}

	update ();
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

	if (event->modifiers() & Qt::ShiftModifier) {
		if (m_selected->count () > 0) {
			int o = m_selected->last ();
			if (o < i) {
				for (int y = o; y <= i; y++) {
					m_selected->append (y);
					m_items->value(y)->setSelected (true);
				}
			} else {
				for (int y = i; y <= o; y++) {
					m_selected->append (y);
					m_items->value(y)->setSelected (true);
				}
			}
		} else {
			m_selected->append (i);
			m_items->value(i)->setSelected (true);
		}
	} else if (event->modifiers () & Qt::ControlModifier) {
		m_items->value(i)->setSelected (true);
		m_selected->append (i);
	} else {
		for (int y = 0; y < m_selected->count(); y++) {
			m_items->value(m_selected->value(y))->setSelected (false);
		}
		m_selected->clear();

		m_items->value(i)->setSelected (true);
		m_selected->append(i);
	}

	update ();
}

void
PlaylistList::paintEvent (QPaintEvent *event)
{
	int i;
	QPainter paint;
	paint.begin (this);
	paint.setFont (*m_font);
	paint.setClipping (false);
	paint.setPen (QPen (m_color_normal));

	int cy = event->rect().y () + m_offset;
	int ch = event->rect().height();
	int sitem = cy / getFontH();
	int eitem = (cy + ch) / getFontH();

	if (eitem > m_items->count())
		eitem = m_items->count();

	for (i = sitem; i < eitem; i++) {
		QRect r (3, getFontH()*(i-sitem), size().width(), getFontH());
		if (event->region().contains (r)) {
			PlaylistItem *item = m_items->value (i);
			QString q;
			q.sprintf ("%d. ", i+1);
			q += item->text ();

			if (item->getSelected ()) {
				paint.fillRect (r, QBrush (m_color_selected));
			} else {
				paint.eraseRect (r);
			}

			if (item->getActive ()) {
				paint.setPen (QPen (m_color_active));
				paint.drawText (r, q);
				paint.setPen (QPen (m_color_normal));
			} else {
				paint.drawText (r, q);
			}

		}
	}

	if (getFontH()*(i-sitem) < size().height()) {
		paint.eraseRect (QRect (0, getFontH()*(i-sitem), size().width(), 
								size().height()-getFontH()*(i-sitem)));
	}

	paint.end ();

}

void
PlaylistList::addItem (PlaylistItem *i)
{
	m_items->append (i);
	m_itemmap->insert (i->getID(), i);
	if (m_items->count()*getFontH () > size().height()) {
		resize (size().width(), m_items->count ()*getFontH ());
	}
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
	m_font->setPixelSize (10);

	if (m_fontmetrics) {
		delete m_fontmetrics;
	}
	m_fontmetrics = new QFontMetrics (*m_font);

	m_color_active.setNamedColor (skin->getPLeditValue ("current"));
	m_color_selected.setNamedColor (skin->getPLeditValue ("selectedbg"));
	m_color_normal.setNamedColor (skin->getPLeditValue ("normal"));

	update ();
}

void
PlaylistList::setSize (int width, int height)
{
	int nx, ny;
	if (width > size().width()) {
		nx = width;
	} else {
		nx = size().width();
	}
	if (height > size().height()) {
		ny = height;
	} else {
		ny = size().height();
	}
	resize (nx, ny);

}



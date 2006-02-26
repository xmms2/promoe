#include "PlaylistList.h"
#include "Playlist.h"

#include <QPaintEvent>

PlaylistItem::PlaylistItem (PlaylistList *pl, uint id)
{
	m_pl = pl;
	m_id = id;
	pl->addItem (this);
}

QString
PlaylistItem::text (void)
{
	return QString::fromUtf8 ("Entry entry entry");
}

PlaylistList::PlaylistList (QWidget *parent) : QWidget (parent)
{
	PlaylistWindow *pl = dynamic_cast<PlaylistWindow*>(window ());
	connect (pl->getSkin (), SIGNAL (skinChanged (Skin *)), this, SLOT (setPixmaps(Skin *)));
	m_font = NULL;
	m_fontmetrics = NULL;
	m_items = new QList<PlaylistItem *>;
	m_offset = 0;
}

void
PlaylistList::paintEvent (QPaintEvent *event)
{
	int i;
	QPainter paint;
	paint.begin (this);
	paint.setFont (*m_font);
	paint.setClipping (false);

	int cy = event->rect().y () + m_offset;
	int ch = event->rect().height();
	int sitem = cy / getFontH();
	int eitem = (cy + ch) / getFontH();

	if (eitem > m_items->count())
		eitem = m_items->count();

	for (i = sitem; i < eitem; i++) {
		QRect r (3, getFontH()*(i-sitem), size().width(), getFontH());
		if (event->region().contains (r)) {
			QString q;
			q.sprintf ("%d. ", i+1);
			q += m_items->value(i)->text ();
			paint.eraseRect (r);
			paint.drawText (r, q);
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
	resize (ny, nx);
}



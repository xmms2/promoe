#include "XMMSHandler.h"

#include "PlaylistShade.h"
#include "Playlist.h"

#include <QSettings>

PlaylistShade::PlaylistShade (QWidget *parent) : QWidget (parent)
{
	QSettings s;
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	if (!s.contains ("playlist/shadedsize"))
		s.setValue ("playlist/shadedsize", 8);

	Skin *skin = Skin::getInstance ();
	setMinimumSize (275, 14);
	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	connect (xmmsh, SIGNAL(currentSong (QHash<QString, QString>)), 
			 this, SLOT(setMediainfo (QHash<QString, QString>)));

	connect (xmmsh, SIGNAL(settingsSaved ()), 
			 this, SLOT(settingsSaved ()));

	m_text = "Promoe 0.1 - A very neat XMMS2 client";
}

void
PlaylistShade::settingsSaved ()
{
	QSettings s;

	m_font.setPixelSize (s.value ("playlist/shadedsize").toInt ());
	update ();
}

void
PlaylistShade::setMediainfo (QHash<QString, QString> h)
{
	QString n;
	if (h.contains ("artist") && h.contains ("album") && h.contains ("title")) {
		n = h.value("artist") + " - " + h.value("album") + " - " + h.value("title");
	} else {
		n = h.value("url");
	}
	m_text = (n);

	update ();
}

void
PlaylistShade::setPixmaps (Skin *skin)
{
	QSettings s;

	m_pixmap_le = skin->getPls (Skin::PLS_WS_LE_0);
	m_pixmap_re_0 = skin->getPls (Skin::PLS_WS_RE_0);
	m_pixmap_re_1 = skin->getPls (Skin::PLS_WS_RE_1);
	m_pixmap_mid = skin->getPls (Skin::PLS_WS_MID_0);
	m_pixmap_re = m_pixmap_re_0;

	m_font = QFont (skin->getPLeditValue ("font"));
	m_font.setPixelSize (s.value ("playlist/shadedsize").toInt ());
	m_color.setNamedColor (skin->getPLeditValue ("normal"));

	update ();

}

void
PlaylistShade::setActive (bool b)
{
	if (b) {
		m_pixmap_re = m_pixmap_re_0;
	} else {
		m_pixmap_re = m_pixmap_re_1;
	}

	update ();
}

void 
PlaylistShade::mouseDoubleClickEvent (QMouseEvent *event)
{
	PlaylistWindow *pw = dynamic_cast<PlaylistWindow*>(window ());
	pw->switchDisplay ();
}

void
PlaylistShade::paintEvent (QPaintEvent *event)
{
	QRect r;

	QPainter p;
	p.begin (this);
	r.setRect (0, 0, m_pixmap_le.size().width(), m_pixmap_le.size().height());
	p.drawPixmap (r, m_pixmap_le, m_pixmap_le.rect());
	int s = size().width() - m_pixmap_le.size().width() - m_pixmap_re.size().width();
	r.setRect (m_pixmap_le.size().width(), 0, s, m_pixmap_mid.size().height());
	p.drawPixmap (r, m_pixmap_mid, m_pixmap_mid.rect ());
	r.setRect (size().width() - m_pixmap_re.size().width(), 0, m_pixmap_re.size().width(),
			   m_pixmap_re.size().height());
	p.drawPixmap (r, m_pixmap_re, m_pixmap_re.rect ());

	/* Text */
	p.setFont (m_font);
	p.setPen (m_color);

	p.drawText (QRect (5, 4, size().width()-25, 7),
				Qt::AlignLeft | Qt::AlignVCenter,
				m_text);

	p.end ();

}


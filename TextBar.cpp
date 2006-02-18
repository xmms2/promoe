#include "MainWindow.h"
#include "Display.h"
#include "TextBar.h"

TextScroller::TextScroller (QWidget *parent, uint w, uint h) : QWidget (parent)
{

	MainWindow *mw = (MainWindow *)((SkinDisplay *)parent)->getMW();
	m_skin = mw->getSkin();

	connect (m_skin, SIGNAL (skinChanged (Skin *)), this, SLOT (setPixmaps(Skin *)));

	m_h = h;
	m_w = w;
	m_x_off = 0;
	m_x2_off = 0;
	
	setMinimumSize(m_w + 2, m_h);
	setMaximumSize(m_w + 2, m_h);

	if (m_h > 10) {
		m_y = 4;
	} else {
		m_y = 0;
	}

	m_timer = new QTimer (this);
	connect (m_timer, SIGNAL (timeout()), this, SLOT (addOffset ()));

	setAutoFillBackground (true);


	//setText (QString::fromUtf8 ("Okerueu etuoduå öästö åntöå dS !! !¤ ¤ % % & & ¤"));
	
}

void
TextScroller::setPixmaps (Skin *skin)
{
	QPalette pal = palette ();
	QBrush b = QBrush (Qt::TexturePattern);
	b.setTexture (skin->getItem (Skin::TEXTBG));
	pal.setBrush (QPalette::Window, b);
	setPalette (pal);

	setText (QString::fromUtf8 ("Promoe 0.1"));
	update();
}
void 
TextScroller::addOffset ()
{
	if (m_x2_off > 0) {
		m_x2_off --;
	} else if (m_x_off < m_pixmap.size().width()) {
		m_x_off ++;
	} else {
		m_x_off = 1;
		m_x2_off = 0;
	}

	update();
	m_timer->start (40);
}

void
TextScroller::setText (const QString &text)
{
	drawQtFont (text);
	m_x_off = 1;
	m_x2_off = 0;
	update ();
}

void
TextScroller::drawBitmapFont (const QString &text)
{
	int width = text.length() * 6;
	QString (temp) = text.toLower ();

	if (width > m_w) {
		temp += QString::fromAscii ("  --  ");
		m_x2_off = m_w / 2;
		m_pixmap = QPixmap (width + 6*6, m_h);
		m_timer->start (40);
	} else {
		m_pixmap = QPixmap (m_w, m_h);
	}
	const char *t = temp.toLatin1();

	QPainter (paint);

	paint.begin (&m_pixmap);
	paint.fillRect (m_pixmap.rect(), Qt::white);
	for (uint i = 0; i < strlen (t); i++) {
		QPixmap p = m_skin->getLetter (t[i]);
		if (!p) {
			p = m_skin->getLetter(' ');
		}

		paint.drawPixmap (QRect (i * 6, m_y, 4, 6),
						  p, p.rect());
	}

	paint.end();
	m_pixmap.setMask (m_pixmap.createHeuristicMask ());

}

void
TextScroller::drawQtFont (const QString &text)
{
	QFont font(m_skin->getPLeditValue ("font"));
	font.setPointSize (6);

	QFontMetrics fM(font);
	QRect rect = fM.boundingRect (text);

	QString (temp) = text;

	if (rect.width() > m_w) {
		temp += QString::fromAscii ("  --  ");
		QRect rect = fM.boundingRect (temp);
		m_timer->start (40);
		m_x2_off = m_w / 2;

		m_pixmap = QPixmap (rect.width(), m_h);
	} else {
		m_pixmap = QPixmap (m_w, m_h);
	}

	QPainter paint;
	paint.begin (&m_pixmap);
	paint.drawPixmap (m_pixmap.rect (),
					  m_skin->getItem (Skin::TEXTBG),
					  m_skin->getItem (Skin::TEXTBG).rect ());

	paint.setFont (font);
	QColor c;
	c.setNamedColor (m_skin->getPLeditValue ("normal"));
	paint.setPen (c);
	paint.drawText (m_pixmap.rect (),
					Qt::AlignLeft | Qt::AlignVCenter,
					temp);
	paint.end ();

}

void 
TextScroller::paintEvent (QPaintEvent *event)
{
	int pad = 0;

	if (m_pixmap.isNull ()) {
		return;
	}

	int w2 = m_pixmap.size().width() - m_x_off;
	if (w2 < m_w) {
		pad = m_w - w2;
	}

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (QRect (m_x2_off, 0, m_w - pad, m_h),
					  m_pixmap, 
					  QRect (m_x_off, 0, m_w, m_h));
	if (pad) {
		paint.drawPixmap (QRect (m_w - pad, 0, pad, m_h),
						  m_pixmap,
						  QRect (0, 0, pad, m_h));
	}
	paint.end ();
}

TextScroller::~TextScroller ()
{
}


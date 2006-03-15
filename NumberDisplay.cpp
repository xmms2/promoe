#include "MainWindow.h"
#include "NumberDisplay.h"

NumberDisplay::NumberDisplay (QWidget *parent, uint w, uint startpx) : PixWidget (parent)
{
	m_w = w;
	m_startpx = startpx;

	m_pixmap = QPixmap (m_w, 13);
	
	setMinimumSize (m_w, 13);
	setMaximumSize (m_w, 13);

	setNumber (0,0);
}

void
NumberDisplay::setPixmaps (Skin *skin)
{
	QBrush b (Qt::TexturePattern);
	b.setTexture (skin->getNumber (10));

	QPainter paint;
	paint.begin (&m_pixmap);
	paint.fillRect (m_pixmap.rect (), b);
	paint.drawPixmap (m_startpx, 0, skin->getNumber (m_n1));
	paint.drawPixmap (m_startpx+12, 0, skin->getNumber (m_n2));
	paint.end();

	update ();
}

void
NumberDisplay::setNumber (uint n1, uint n2)
{
	if (m_n1 != n1 || m_n2 != n2) {
		Skin *skin = Skin::getInstance ();

		m_n1 = n1;
		m_n2 = n2;

		setPixmaps (skin);
	}
}

NumberDisplay::~NumberDisplay ()
{
}
void
NumberDisplay::mousePressEvent (QMouseEvent *event)
{
}
void
NumberDisplay::mouseReleaseEvent (QMouseEvent *event)
{
	emit clicked();
}


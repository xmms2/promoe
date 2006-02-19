#include "SmallNumberDisplay.h"

SmallNumberDisplay::SmallNumberDisplay (QWidget *parent, int w) : PixWidget (parent)
{
	m_w = w;
	setMinimumSize (w, 6);
	setMaximumSize (w, 6);
	m_pixmap = QPixmap (w, 6);
}

void
SmallNumberDisplay::setPixmaps (Skin *skin)
{
	m_skin = skin;
	drawNumber ();
}

void
SmallNumberDisplay::setNumber (char i1, char i2, char i3)
{
	m_nums[0] = i1;
	m_nums[1] = i2;
	m_nums[2] = i3;
	m_num = 3;

	drawNumber ();

	update ();
}

void
SmallNumberDisplay::setNumber (char i1, char i2)
{
	m_nums[0] = i1;
	m_nums[1] = i2;
	m_num = 2;

	drawNumber ();

	update ();
}

void
SmallNumberDisplay::drawNumber ()
{

	if (!m_skin) {
		return;
	}

	QPainter paint;
	paint.begin (&m_pixmap);
	paint.drawPixmap (m_pixmap.rect (),
					  m_skin->getItem (Skin::TEXTBG),
					  m_pixmap.rect ());

	for (int i = 0; i < m_num; i++) {
		paint.drawPixmap (QRect (i*5, 0, 4, 6),
						  m_skin->getLetter (m_nums[i]),
						  m_skin->getLetter (m_nums[i]).rect ());
	}

	paint.end ();
}


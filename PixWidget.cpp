#include "PixWidget.h"


PixWidget::PixWidget (QWidget *parent) : QWidget (parent)
{
	m_pixmap = QPixmap(0,0);
}


PixWidget::~PixWidget ()
{
}


/*
 * Since almost every Widget I have here
 * is done by drawing self.pixmap onto self
 * we define this a generic PaintEvent handler
 */
void 
PixWidget::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull () || !m_pixmap) {
		return;
	}

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (rect (), m_pixmap, m_pixmap.rect ());
	paint.end ();
}

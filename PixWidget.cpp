#include "MainWindow.h"

PixWidget::PixWidget (QWidget *parent) : QWidget (parent)
{
	Skin *skin = Skin::getInstance();
	m_pixmap = QPixmap(0,0);

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));
}


PixWidget::~PixWidget ()
{
}


void
PixWidget::setPixmaps(Skin *skin)
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
	if (m_pixmap.isNull ()) {
		return;
	}

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (rect (), m_pixmap, m_pixmap.rect ());
	paint.end ();
}

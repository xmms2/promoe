#include "MainWindow.h"
#include "Display.h"

SkinDisplay::SkinDisplay (QWidget *parent) : QWidget(parent)
{
	Skin *skin = Skin::getInstance ();

	m_mw = parent;

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps (Skin *)));
}

void
SkinDisplay::setPixmaps (Skin *skin)
{
}

void 
SkinDisplay::mousePressEvent (QMouseEvent *event)
{
	dynamic_cast<MainWindow *>(getMW())->raisePL ();
	m_diffX = event->x();
	m_diffY = event->y();
}

void 
SkinDisplay::mouseMoveEvent (QMouseEvent *event)
{
	m_mw->move(event->globalPos().x() - m_diffX,
			   event->globalPos().y() - m_diffY);
}

void
SkinDisplay::enterEvent (QEvent *event)
{
	dynamic_cast<TitleBar *>(m_tbar)->setActive(true);
}

void
SkinDisplay::leaveEvent (QEvent *event)
{
	dynamic_cast<TitleBar *>(m_tbar)->setActive(false);
}

void 
SkinDisplay::paintEvent (QPaintEvent *event)
{
	QPainter(paint);

	paint.begin(this);
	paint.eraseRect(rect());
	paint.end();
}

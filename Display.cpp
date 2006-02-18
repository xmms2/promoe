#include "Display.h"
#include "MainWindow.h"

SkinDisplay::SkinDisplay (QWidget *parent) : QWidget(parent)
{
	m_noDrag = false;
	m_mw = parent;

	connect(((MainWindow *)m_mw)->getSkin(), SIGNAL(skinChanged(Skin *)), this, SLOT(setPixmaps(Skin *)));
}

void
SkinDisplay::setPixmaps (Skin *skin)
{
}

void 
SkinDisplay::mousePressEvent (QMouseEvent *event)
{
	m_diffX = event->x();
	m_diffY = event->y();
}

void 
SkinDisplay::mouseMoveEvent (QMouseEvent *event)
{
	if (!m_noDrag) {
		m_mw->move(event->globalPos().x() - m_diffX,
				   event->globalPos().y() - m_diffY);
	}
}

void
SkinDisplay::enterEvent (QEvent *event)
{
	m_tbar->setActive(true);
}

void
SkinDisplay::leaveEvent (QEvent *event)
{
	m_tbar->setActive(false);
}

void 
SkinDisplay::paintEvent (QPaintEvent *event)
{
	QPainter(paint);

	paint.begin(this);
	paint.eraseRect(rect());
	paint.end();
}

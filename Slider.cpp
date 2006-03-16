#include "MainWindow.h"

#include "Slider.h"
#include "Skin.h"

#include <QWidget>
#include <QMouseEvent>

BarButton::BarButton (QWidget *parent, uint normal, uint pressed) : Button (parent, normal, pressed)
{
	m_slider = dynamic_cast<Slider *>(parent);
	setMinimumSize (29, 10);
	setMaximumSize (29, 10);
	m_moving = false;
}

void
BarButton::setPos (uint pos)
{
	m_pos = pos;
	if (!m_moving) {
		move (pos, 0);
	}
}

uint
BarButton::getPos (void)
{
	return m_pos;
}

void
BarButton::mousePressEvent (QMouseEvent *event)
{
	m_moving = true;
}

void
BarButton::mouseReleaseEvent (QMouseEvent *event)
{
	m_moving = false;

	float value = pos().x() / (float)(m_slider->width() - width());

	m_slider->requestPos (value);
}

void
BarButton::mouseMoveEvent (QMouseEvent *event)
{
	QPoint p (event->pos ());

	/** @todo this could be cleaned up */
	if (m_slider->getVertical ()) {
		int npos = pos().x() + p.x() - m_diffx;
		if (npos >= 0 && npos + rect().width() <= m_slider->rect().width()) {
			move (npos, 0);
		} else if (npos < 0) {
			move (0, 0);
		} else if (npos + rect().width() > m_slider->rect().width()) {
			move (m_slider->rect().width()-rect().width(), 0);
		}
	} else {
		int npos = pos().y()+p.y()-m_diffy;
		if (npos >= 0 && npos + rect().height() <= m_slider->rect().height()) {
			move (npos, 0);
		} else if (npos < 0) {
			move (0, 0);
		} else if (npos + rect().height() > m_slider->rect().height()) {
			move (m_slider->rect().height()-rect().height(), 0);
		}
	}

}


Slider::Slider (QWidget *parent, uint bg, uint bnormal, uint bpressed, bool vertical) : PixWidget (parent)
{
	m_bg = bg;
	m_vertical = vertical;

	setMinimumSize (248, 10);
	setMaximumSize (248, 10);

	m_max = 0;

	m_button = new BarButton (this, bnormal, bpressed);
	m_button->move (0, 0);

	if (m_vertical) {
		m_pix = size().width()-m_button->size().width();
	} else {
		m_pix = size().height()-m_button->size().height();
	}
	
	hideBar (true);

}

void
Slider::setSize (uint x, uint y)
{
	setMinimumSize (x, y);
	setMaximumSize (x, y);

	if (m_vertical) {
		m_pix = size().width()-m_button->size().width();
	} else {
		m_pix = size().height()-m_button->size().height();
	}

}

uint
Slider::getPos (void)
{

	if (m_vertical) {
		return (uint)((float)m_button->pos().x()) / (float)(m_pix) * float(m_max);
	} else {
		return (uint)((float)m_button->pos().y())/((float)(m_pix*m_max));
	}
}

void
Slider::hideBar (bool b)
{
	
	if (b) { 
		m_button->hide (); 
	} else { 
		m_button->show (); 
	} 
	update(); 

}

void
Slider::setPos (uint p)
{
	if (!m_max) {
		return;
	}

	uint x = m_pix * p / m_max;
	if (x < m_pix - m_button->rect().width() && x != m_button->getPos()) {
		m_button->setPos (x);
		update ();
	}
}

void
Slider::requestPos (float value)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance();

	uint new_pos = (uint)(m_max * value);
	xmmsh->setPlaytime (new_pos);
}

void
Slider::setPixmaps (Skin *skin)
{
	m_pixmap = skin->getItem (m_bg);
}


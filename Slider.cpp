#include "MainWindow.h"

#include "Slider.h"

#include <QWidget>
#include <QMouseEvent>

BarButton::BarButton (QWidget *parent, uint normal, uint pressed) : Button (parent, normal, pressed)
{
	m_slider = dynamic_cast<Slider *>(parent);
	setMinimumSize (29, 10);
	setMaximumSize (29, 10);
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
	MainWindow *mw = dynamic_cast<MainWindow *>(window ());

	m_bg = bg;
	m_vertical = vertical;

	setMinimumSize (248, 10);
	setMaximumSize (248, 10);

	m_max = 0;

	m_button = new BarButton (this, bnormal, bpressed);
	m_button->move (0, 0);
	connect (m_button, SIGNAL(clicked ()), mw->getHandler (), SLOT (setPlaytime ()));

	if (m_vertical) {
		m_pix = size().width()-m_button->size().width();
	} else {
		m_pix = size().height()-m_button->size().height();
	}
	
	m_pos = 0;

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
	if (x < m_pix - m_button->rect().width() && x != m_pos) {
		m_button->move (x , 0);
		m_pos = x;
		update ();
	}

}

void
Slider::setPixmaps (Skin *skin)
{
	m_pixmap = skin->getItem (m_bg);
}


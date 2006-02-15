#include "Button.h"
#include "MainWindow.h"
#include "Display.h"

Button::Button (QWidget *parent, uint normal, uint pressed) : PixWidget (parent)
{
	MainWindow *mw = (MainWindow *)((SkinDisplay *)parent)->getMW();

	m_pixmap_normal = mw->getSkin ()->getItem (normal);
	m_pixmap_pressed = mw->getSkin ()->getItem (pressed);
	m_pixmap = m_pixmap_normal;
	m_func = NULL;

	setMinimumSize (m_pixmap.size ());
	setMaximumSize (m_pixmap.size ());
}

Button::~Button ()
{
}

void Button::mousePressEvent (QMouseEvent *event)
{
	((SkinDisplay *)parent ())->setNoDrag (true);
	m_pixmap = m_pixmap_pressed;
	update ();
}

void Button::mouseReleaseEvent (QMouseEvent *event)
{
	((SkinDisplay *)parent())->setNoDrag (false);
	m_pixmap = m_pixmap_normal;
	update();

	if (m_func) {
		m_func (m_userdata);
	}
}

void
clicked (void *userdata)
{
	ToggleButton *t = (ToggleButton *)userdata;
	t->toggleOn (!t->getOn());
	t->runFunc ();
}

ToggleButton::ToggleButton (QWidget *parent, uint on_normal, uint on_pressed,
							uint off_normal, uint off_pressed) : 
					Button (parent, off_normal, off_pressed)
{
	MainWindow *mw = (MainWindow *)((SkinDisplay *)parent)->getMW();

	m_pixmap_on_normal = mw->getSkin()->getItem(on_normal);
	m_pixmap_on_pressed = mw->getSkin()->getItem(on_pressed);
	m_pixmap_off_normal = mw->getSkin()->getItem(off_normal);
	m_pixmap_off_pressed = mw->getSkin()->getItem(off_pressed);

	m_func = clicked;
	m_userdata = (void *)this;

	m_func2 = NULL;
	m_userdata2 = NULL;
	
	toggleOn (false);
}

void
ToggleButton::runFunc ()
{
	if (m_func2) {
		m_func2 (m_userdata2);
	}
}

void
ToggleButton::toggleOn (bool t)
{
	if (t) {
		m_pixmap_normal = m_pixmap_on_normal;
		m_pixmap_pressed = m_pixmap_on_pressed;
		m_toggled_on = true;
	} else {
		m_pixmap_normal = m_pixmap_off_normal;
		m_pixmap_pressed = m_pixmap_off_pressed;
		m_toggled_on = false;
	}

	m_pixmap = m_pixmap_normal;
	update ();

}

ToggleButton::~ToggleButton ()
{
}

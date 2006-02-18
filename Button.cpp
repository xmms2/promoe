#include "Button.h"
#include "Display.h"

Button::Button (QWidget *parent, uint normal, uint pressed) : PixWidget (parent)
{
	m_name_normal = normal;
	m_name_pressed = pressed;
}

Button::~Button ()
{
}

void
Button::setPixmaps(Skin *skin)
{
	m_pixmap_normal = skin->getItem (m_name_normal);
	m_pixmap_pressed = skin->getItem (m_name_pressed);
	m_pixmap = m_pixmap_normal;
	m_func = NULL;

	setMinimumSize (m_pixmap.size ());
	setMaximumSize (m_pixmap.size ());

	update();
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

	m_name_on_normal = on_normal;
	m_name_on_pressed = on_pressed;
	m_name_off_normal = off_normal;
	m_name_off_pressed = off_pressed;

	m_func = clicked;
	m_userdata = (void *)this;

	m_func2 = NULL;
	m_userdata2 = NULL;
}


void
ToggleButton::setPixmaps(Skin *skin)
{
	m_pixmap_on_normal = skin->getItem(m_name_on_normal);
	m_pixmap_on_pressed = skin->getItem(m_name_on_pressed);
	m_pixmap_off_normal = skin->getItem(m_name_off_normal);
	m_pixmap_off_pressed = skin->getItem(m_name_off_pressed);

	toggleOn(m_toggled_on);

	setMinimumSize (m_pixmap.size ());
	setMaximumSize (m_pixmap.size ());

	update();
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

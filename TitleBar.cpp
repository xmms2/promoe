#include "MainWindow.h"
#include "TitleBar.h"
#include "Display.h"

TitleBar::TitleBar (QWidget *parent, bool shaded) : PixWidget (parent)
{
	MainWindow *mw = dynamic_cast<MainWindow*>(window ());
	m_shaded = shaded;
	
	setMinimumSize (275, 14);
	setMaximumSize (275, 14);

	m_menubtn = new Button (this, Skin::MENUBUTTON_0, Skin::MENUBUTTON_1);
	m_menubtn->move(6, 3);

	m_minimize = new Button (this, Skin::MINIMIZE_0, Skin::MINIMIZE_1);
	connect (m_minimize, SIGNAL (clicked ()), mw, SLOT (showMinimized ()));
	m_minimize->move(244, 3);

	m_shadebtn = new Button (this, Skin::SHADE_1_0, Skin::SHADE_1_1);
	connect (m_shadebtn, SIGNAL (clicked()), mw, SLOT (switchDisplay ()));
	m_shadebtn->move(254, 3);

	m_closebtn = new Button (this, Skin::CLOSE_0, Skin::CLOSE_1);
	connect (m_closebtn, SIGNAL (clicked()), qApp, SLOT (quit ()));
	m_closebtn->move(264, 3);

}

void
TitleBar::setPixmaps (Skin *skin)
{
	if (m_shaded) {
		m_pixmap_active = skin->getItem (Skin::STATUSBAR_0);
		m_pixmap_inactive = skin->getItem (Skin::STATUSBAR_1);
	} else { 
		m_pixmap_active = skin->getItem (Skin::TITLEBAR_0);
		m_pixmap_inactive = skin->getItem (Skin::TITLEBAR_1);
	}

	if (underMouse()) {
		m_pixmap = m_pixmap_active;
	} else {
		m_pixmap = m_pixmap_inactive;
	}

	update ();
}

TitleBar::~TitleBar ()
{
}

void 
TitleBar::setActive (bool active)
{
	if (active) {
		m_pixmap = m_pixmap_active;
		update ();
	} else {
		m_pixmap = m_pixmap_inactive;
		update ();
	}
}

void 
TitleBar::mouseDoubleClickEvent (QMouseEvent *event)
{
	MainWindow *mw = dynamic_cast<MainWindow*>(window ());
	mw->switchDisplay ();
}


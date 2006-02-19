#include "MainWindow.h"
#include "TitleBar.h"
#include "Display.h"

TitleBar::TitleBar (QWidget *parent, bool shaded) : PixWidget (parent)
{
	m_shaded = shaded;
	
	setMinimumSize(QSize(275, 14));
	setMaximumSize(QSize(275, 14));
}

void
TitleBar::setPixmaps (Skin *skin)
{
	if (m_shaded) {
		m_pixmap_active = skin->getItem(Skin::STATUSBAR_0);
		m_pixmap_inactive = skin->getItem(Skin::STATUSBAR_1);
	} else {
		m_pixmap_active = skin->getItem(Skin::TITLEBAR_0);
		m_pixmap_inactive = skin->getItem(Skin::TITLEBAR_1);
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
		update();
	} else {
		m_pixmap = m_pixmap_inactive;
		update();
	}
}

void 
TitleBar::mouseDoubleClickEvent (QMouseEvent *event)
{
}

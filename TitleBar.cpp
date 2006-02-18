#include "MainWindow.h"
#include "TitleBar.h"
#include "Display.h"

TitleBar::TitleBar (QWidget *parent, bool shaded) : PixWidget (parent)
{
	MainWindow *mw = (MainWindow *)((SkinDisplay *)parent)->getMW();

	if (shaded) {
		m_pixmap_active = mw->getSkin()->getItem(Skin::STATUSBAR_0);
		m_pixmap_inactive = mw->getSkin()->getItem(Skin::STATUSBAR_1);
	} else {
		m_pixmap_active = mw->getSkin()->getItem(Skin::TITLEBAR_0);
		m_pixmap_inactive = mw->getSkin()->getItem(Skin::TITLEBAR_1);
	}


	if (underMouse()) {
		m_pixmap = m_pixmap_active;
	} else {
		m_pixmap = m_pixmap_inactive;
	}
	
	qDebug("%d %d", m_pixmap.width(), m_pixmap.height());

	setMinimumSize(QSize(275, 14));
	setMaximumSize(QSize(275, 14));
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

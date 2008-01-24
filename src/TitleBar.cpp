#include "MainWindow.h"
#include "TitleBar.h"
#include "Display.h"
#include "SkinChooser.h"
// #include "MedialibWindow.h"
#include "settingsdialog.h"
#include "Button.h"
#include "BrowseDialog.h"

#include <QMenu>

TitleBar::TitleBar (QWidget *parent, bool shaded) : PixWidget (parent)
{
	MainWindow *mw = dynamic_cast<MainWindow*>(window ());
	m_shaded = shaded;
	
	setMinimumSize (275, 14);
	setMaximumSize (275, 14);

	m_menubtn = new Button (this, Skin::MENUBUTTON_0, Skin::MENUBUTTON_1);
	connect (m_menubtn, SIGNAL (clicked ()), this, SLOT (showMenu ()));
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
TitleBar::showMenu (void)
{
	QMenu qm(this);

	QAction *a;

	a = new QAction (tr ("Medialib browser"), this);
	a->setShortcut (tr ("Alt+M"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showMlib ()));
	a->setEnabled(false); // FIXME: disabled for now, as Mlib-browser doesn't work
	qm.addAction (a);
	a = new QAction (tr ("Server-side browser"), this);
	a->setShortcut (tr ("Alt+S"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showServerB ()));
	qm.addAction (a);
	qm.addSeparator ();
	a = new QAction (tr ("Theme settings"), this);
	a->setShortcut (tr ("Alt+T"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showTheme ()));
	qm.addAction (a);
	a = new QAction (tr ("Application settings"), this);
	a->setShortcut (tr ("Alt+A"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showSettings ()));
	qm.addAction (a);
	a = new QAction (tr ("Server settings"), this);
	a->setShortcut (tr ("Alt+S"));
	a->setEnabled(false); // FIXME: disabled for now, not yet implemented
	qm.addAction (a);
	qm.addSeparator ();
	a = new QAction (tr ("Quit"), this);
	a->setShortcut (tr ("Ctrl+Q"));
	connect (a, SIGNAL (triggered ()), qApp, SLOT (quit ()));
	qm.addAction (a);

	qm.exec(QPoint (window()->pos().x()+6, window()->pos().y()+3));

}

void
TitleBar::showMlib ()
{
	/*
	MedialibWindow *mw = new MedialibWindow (window ());
	mw->show ();
	*/
}

void
TitleBar::showServerB ()
{
	BrowseDialog *bd = new BrowseDialog (window ());
	bd->show ();
}

void
TitleBar::showSettings ()
{
	SettingsDialog *sw = new SettingsDialog (window ());
	sw->show ();
}

void
TitleBar::showTheme ()
{
	SkinChooser *sk = new SkinChooser (window());
	sk->show();
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


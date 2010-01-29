/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2010 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "mainwindow.h"
#include "titlebar.h"
#include "skindisplay.h"
#include "SkinChooser.h"
// #include "MedialibWindow.h"
#include "settingsdialog.h"
#include "pixmapbutton.h"
#include "BrowseDialog.h"
#include "Skin.h"
#include "skinmanager.h"

#include <QMenu>
#include <QPainter>

TitleBar::TitleBar (QWidget *parent, bool shaded) : QWidget (parent)
{
	MainWindow *mw = dynamic_cast<MainWindow*>(window ());
	m_shaded = shaded;
	Skin *skin = SkinManager::instance ()->activeSkin ();

	setFixedSize (275, 14);

	m_menubtn = new PixmapButton (this);
	m_menubtn->resize (skin->getSize (Skin::BUTTON_MW_MENU));
	m_menubtn->move (skin->getPos (Skin::BUTTON_MW_MENU));
	connect (m_menubtn, SIGNAL (clicked ()), this, SLOT (showMenu ()));

	m_minimize = new PixmapButton (this);
	m_minimize->resize (skin->getSize (Skin::BUTTON_MW_MINIMIZE));
	m_minimize->move (skin->getPos (Skin::BUTTON_MW_MINIMIZE));
	connect (m_minimize, SIGNAL (clicked ()), mw, SLOT (showMinimized ()));

	if (shaded) {
		m_shadebtn = new PixmapButton (this);
		m_shadebtn->resize (skin->getSize (Skin::BUTTON_MW_SHADED_UNSHADE));
		m_shadebtn->move (skin->getPos (Skin::BUTTON_MW_SHADED_UNSHADE));
	} else {
		m_shadebtn = new PixmapButton (this);
		m_shadebtn->resize (skin->getSize (Skin::BUTTON_MW_SHADE));
		m_shadebtn->move (skin->getPos (Skin::BUTTON_MW_SHADE));
	}
	connect (m_shadebtn, SIGNAL (clicked()), mw, SLOT (switchDisplay ()));

	m_closebtn = new PixmapButton (this);
	m_closebtn->resize (skin->getSize (Skin::BUTTON_MW_CLOSE));
	m_closebtn->move (skin->getPos (Skin::BUTTON_MW_CLOSE));
	connect (m_closebtn, SIGNAL (clicked()), qApp, SLOT (quit ()));

	m_pixmap = QPixmap(0,0);
	connect (SkinManager::instance (), SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));
}

void
TitleBar::showMenu (void)
{
	QMenu qm(this);

	QAction *a;

	a = new QAction (tr ("Medialib browser"), this);
//	a->setShortcut (tr ("Alt+M"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showMlib ()));
	a->setEnabled(false); // FIXME: disabled for now, as Mlib-browser doesn't work
	qm.addAction (a);
	a = new QAction (tr ("Server-side browser"), this);
//	a->setShortcut (tr ("Alt+S"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showServerB ()));
	qm.addAction (a);
	qm.addSeparator ();
	a = new QAction (tr ("Theme settings"), this);
//	a->setShortcut (tr ("Alt+T"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showTheme ()));
	qm.addAction (a);
	a = new QAction (tr ("Application settings"), this);
//	a->setShortcut (tr ("Alt+A"));
	connect (a, SIGNAL (triggered ()), this, SLOT (showSettings ()));
	qm.addAction (a);
	a = new QAction (tr ("Server settings"), this);
//	a->setShortcut (tr ("Alt+S"));
	a->setEnabled(false); // FIXME: disabled for now, not yet implemented
	qm.addAction (a);
	qm.addSeparator ();
	a = new QAction (tr ("Quit"), this);
//	a->setShortcut (tr ("Ctrl+Q"));
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

	m_menubtn->setIcon (skin->getIcon (Skin::BUTTON_MW_MENU));
	m_minimize->setIcon (skin->getIcon (Skin::BUTTON_MW_MINIMIZE));
	if (m_shaded) {
		m_shadebtn->setIcon (skin->getIcon (Skin::BUTTON_MW_SHADED_UNSHADE));
	} else {
		m_shadebtn->setIcon (skin->getIcon (Skin::BUTTON_MW_SHADE));
	}
	m_closebtn->setIcon (skin->getIcon (Skin::BUTTON_MW_CLOSE));

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
TitleBar::paintEvent (QPaintEvent *event)
{
	QPainter p;
	p.begin (this);
	p.drawPixmap (rect (), m_pixmap);
	p.end ();
}

void 
TitleBar::mouseDoubleClickEvent (QMouseEvent *event)
{
	MainWindow *mw = dynamic_cast<MainWindow*>(window ());
	mw->switchDisplay ();
}

#include "titlebar.moc"

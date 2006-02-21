#include "ShadedDisplay.h"

ShadedDisplay::ShadedDisplay (QWidget *parent) : SkinDisplay (parent)
{
	MainWindow *mw = dynamic_cast<MainWindow*>(parent);
	m_mw = parent;

	setMinimumSize (275, 14);
	setMaximumSize (275, 14);

	m_tbar = new TitleBar(this, true);
	m_tbar->move (0, 0);

	m_number = new SmallNumberDisplay (this, 10);
	m_number->move (135, 4);
	m_number->setNumber (0, 2);

	m_number2 = new SmallNumberDisplay (this, 10);
	m_number2->move (147, 4);
	m_number2->setNumber (0, 2);

	m_title = new TextScroller (this, 48, 7);
	m_title->move (79, 4);
	m_title->setText ("Promoe 0.1");

	m_prev = new Button (this);
	m_prev->move(169, 4);
	m_prev->resize (8, 7);
	connect (m_prev, SIGNAL(clicked()), mw->getHandler (), SLOT(prev()));
	
	m_play = new Button (this);
	m_play->move(177, 4);
	m_play->resize (10, 7);
	connect (m_play, SIGNAL(clicked()), mw->getHandler (), SLOT(play()));

	m_pause = new Button (this);
	m_pause->move(187, 4);
	m_pause->resize (10, 7);
	connect (m_pause, SIGNAL(clicked()), mw->getHandler (), SLOT(pause()));

	m_stop = new Button (this);
	m_stop->move(197, 4);
	m_stop->resize (9, 7);
	connect (m_stop, SIGNAL(clicked()), mw->getHandler (), SLOT(stop()));

	m_next = new Button (this);
	m_next->move(206, 4);
	m_next->resize (8, 7);
	connect (m_next, SIGNAL(clicked()), mw->getHandler (), SLOT(next()));

	m_eject = new Button (this);
	m_eject->move(216, 4);
	m_eject->resize (9, 7);

}


#include "MainWindow.h"
#include "SettingsWindow.h"

#include <QCheckBox>
#include <QSettings>

SettingsWindow::SettingsWindow (QWidget *parent) : QMainWindow (parent)
{
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif
	
	resize (400, 300);

	QWidget *dummy = new QWidget (this);
	setCentralWidget (dummy);

	QVBoxLayout *vbox = new QVBoxLayout (dummy);

	QTabWidget *tab = new QTabWidget (dummy);
	vbox->addWidget (tab);

	QWidget *dummy2 = new QWidget (dummy);
	QHBoxLayout *hbox = new QHBoxLayout (dummy2);
	vbox->addWidget (dummy2);

	QPushButton *cancel = new QPushButton (tr ("Cancel"));
	cancel->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	QPushButton *ok = new QPushButton (tr ("OK"));
	ok->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	connect (ok, SIGNAL (clicked ()), this, SLOT (okButton ()));

	hbox->addWidget (new QWidget (dummy2), 1);
	hbox->addWidget (cancel);
	hbox->addWidget (ok);

	m_mainwindow = new SettingsTabMain (tab);
	m_playlistwin = new QWidget (tab);
	m_medialib = new QWidget (tab);

	tab->addTab (m_mainwindow, tr ("Main Window"));
	tab->addTab (m_playlistwin, tr ("Playlist Window"));
	tab->addTab (m_medialib, tr ("Medialib"));
}

void
SettingsWindow::okButton (void)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();
	m_mainwindow->saveSettings ();
	
	close ();
	xmmsh->updateSettings ();
}

SettingsTabMain::SettingsTabMain (QWidget *parent) : QWidget (parent)
{
	QSettings s;

	QVBoxLayout *vbox = new QVBoxLayout (this);
	QWidget *c = new QWidget (this);
	QHBoxLayout *h = new QHBoxLayout (c);

	vbox->addWidget (c);
	
	s.beginGroup ("maindisplay");
	m_scrolltbar = new QCheckBox (tr ("Scroll titlebar"), this);

	if (s.value("scrollbar").toBool ()) 
		m_scrolltbar->setCheckState (Qt::Checked);
	else
		m_scrolltbar->setCheckState (Qt::Unchecked);

	h->addWidget (m_scrolltbar);

	m_fontsize = new QSpinBox (this);
	m_fontsize->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	m_fontsize->setMinimum (6);
	m_fontsize->setMaximum (20);
	m_fontsize->setValue (s.value("fontsize").toInt ());
	h->addWidget (m_fontsize);

	QLabel *l = new QLabel (tr ("Titlebar fontsize"), this);
	l->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	h->addWidget (l);

	QFrame *f = new QFrame (this);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	vbox->addWidget (f, 1);

	c = new QWidget (this);
	h = new QHBoxLayout (c);

	vbox->addWidget (c);

	m_mainttf = new QCheckBox (tr ("Draw main title with TrueType fonts"), this);
	m_mainttf->setCheckState (s.value ("mainttf").toBool () ? Qt::Checked : Qt::Unchecked);
	h->addWidget (m_mainttf);

	/*
	c = new QWidget (this);
	h = new QHBoxLayout (c);

	vbox->addWidget (c);

	m_shadettf = new QCheckBox (tr ("Draw shaded title with TrueType fonts"), this);
	m_shadettf->setCheckState (s.value ("shadettf").toBool () ? Qt::Checked : Qt::Unchecked);
	h->addWidget (m_shadettf);
	*/

	f = new QFrame (this);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	vbox->addWidget (f, 1);

	c = new QWidget (this);
	h = new QHBoxLayout (c);
	
	vbox->addWidget (c);

	m_quitonclose = new QCheckBox (tr ("Quit XMMS2D when closing Promoe"));
	if (s.contains ("quitonclose"))
		s.setValue ("quitonclose", false);
	m_quitonclose->setCheckState (s.value ("quitonclose").toBool () ? Qt::Checked : Qt::Unchecked);
	h->addWidget (m_quitonclose);
	
	
	s.endGroup ();

}

void
SettingsTabMain::saveSettings (void)
{
	QSettings s;

	s.beginGroup ("maindisplay");
	s.setValue ("scrollbar", m_scrolltbar->checkState () == Qt::Checked);
	s.setValue ("fontsize", m_fontsize->value ());
	s.setValue ("quitonclose", m_quitonclose->checkState () == Qt::Checked);
	s.setValue ("mainttf", m_mainttf->checkState () == Qt::Checked);
	s.endGroup ();
}

#include "MainWindow.h"
#include "SettingsWindow.h"

#include <QCheckBox>
#include <QSettings>

SettingsWindow::SettingsWindow (QWidget *parent) : QMainWindow (parent)
{
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif
	setWindowTitle ("Promoe - Settings window");
	
	resize (400, 500);

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
	connect (cancel, SIGNAL (clicked ()), this, SLOT (close ()));

	hbox->addWidget (new QWidget (dummy2), 1);
	hbox->addWidget (cancel);
	hbox->addWidget (ok);

	m_mainwindow = new SettingsTabMain (NULL);
	m_playlistwin = new SettingsTabPlaylist (NULL);
	m_medialib = new SettingsTabMedialib (NULL);

	tab->addTab (m_mainwindow, tr ("Main Window"));
	tab->addTab (m_playlistwin, tr ("Playlist Window"));
	tab->addTab (m_medialib, tr ("Medialib"));
}

void
SettingsWindow::okButton (void)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();
	m_mainwindow->saveSettings ();
	m_playlistwin->saveSettings ();
	m_medialib->saveSettings ();
	
	close ();
	xmmsh->updateSettings ();
}

SettingsTabMedialib::SettingsTabMedialib (QWidget *parent) : QWidget (parent)
{
	QSettings s;

	if (!s.contains ("medialib_artist/size")) {
		s.setValue ("medialib_artist/size", tr ("Large"));
	}

	if (!s.contains ("medialib_album/size")) {
		s.setValue ("medialib_album/size", tr ("Large"));
	}

	if (!s.contains ("medialib_song/size")) {
		s.setValue ("medialib_song/size", tr ("Large"));
	}

	s.beginGroup("medialib");
	QWidget *dummy = new QWidget (this);

	QVBoxLayout *vbox = new QVBoxLayout (dummy);
	QWidget *c = new QWidget (dummy);
	QHBoxLayout *h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	m_selected = new QComboBox (c);
	m_selected->addItem (tr ("Artists"));
	m_selected->addItem (tr ("Albums"));
	m_selected->addItem (tr ("Songs"));
	m_selected->setEditable (false);

	QString curr = s.value ("selected").toString ();
	for (int i = 0; i < m_selected->count(); i++) {
		if (m_selected->itemText (i) == curr) {
			m_selected->setCurrentIndex (i);
			break;
		}
	}

	h->addWidget (m_selected);

	QLabel *l = new QLabel (tr ("Selected tab on startup"), c);
	h->addWidget (l, 1);

	s.endGroup ();

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	s.beginGroup ("medialib_artist");

	l = new QLabel (tr ("Artist view"), c);
	
	QFrame *f = new QFrame (c);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	h->addWidget (l);
	h->addWidget (f, 1);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	m_artistsize = new QComboBox (c);
	m_artistsize->addItem (tr ("None"));
	m_artistsize->addItem (tr ("Small"));
	m_artistsize->addItem (tr ("Large"));
	m_artistsize->setEditable (false);

	curr = s.value ("size").toString ();
	for (int i = 0; i < m_artistsize->count(); i++) {
		if (m_artistsize->itemText (i) == curr) {
			m_artistsize->setCurrentIndex (i);
			break;
		}
	}

	h->addWidget (m_artistsize);
	l = new QLabel (tr ("Size of media art icon"), c);
	h->addWidget (l, 1);

	s.endGroup ();

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);
	vbox->addWidget (c, 1);

	s.beginGroup ("medialib_album");

	l = new QLabel (tr ("Album view"), c);
	
	f = new QFrame (c);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	h->addWidget (l);
	h->addWidget (f, 1);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);
	
	vbox->addWidget (c, 1);

	m_albumsize = new QComboBox (c);
	m_albumsize->addItem (tr ("None"));
	m_albumsize->addItem (tr ("Small"));
	m_albumsize->addItem (tr ("Large"));
	m_albumsize->setEditable (false);

	curr = s.value ("size").toString ();
	for (int i = 0; i < m_artistsize->count(); i++) {
		if (m_albumsize->itemText (i) == curr) {
			m_albumsize->setCurrentIndex (i);
			break;
		}
	}

	h->addWidget (m_albumsize);
	l = new QLabel (tr ("Size of media art icon"), c);
	h->addWidget (l, 1);

	s.endGroup ();

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);
	vbox->addWidget (c, 1);

	s.beginGroup ("medialib_song");

	l = new QLabel (tr ("Song view"), c);
	
	f = new QFrame (c);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	h->addWidget (l);
	h->addWidget (f, 1);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);
	
	vbox->addWidget (c, 1);

	m_songsize = new QComboBox (c);
	m_songsize->addItem (tr ("None"));
	m_songsize->addItem (tr ("Small"));
	m_songsize->addItem (tr ("Large"));
	m_songsize->setEditable (false);

	curr = s.value ("size").toString ();
	for (int i = 0; i < m_artistsize->count(); i++) {
		if (m_songsize->itemText (i) == curr) {
			m_songsize->setCurrentIndex (i);
			break;
		}
	}

	h->addWidget (m_songsize);
	l = new QLabel (tr ("Size of media art icon"), c);
	h->addWidget (l, 1);

	s.endGroup ();

}

void
SettingsTabMedialib::saveSettings ()
{
	QSettings s;
	s.setValue ("medialib/selected", m_selected->currentText ());
	s.setValue ("medialib_artist/size", m_artistsize->currentText ());
	s.setValue ("medialib_album/size", m_albumsize->currentText ());
	s.setValue ("medialib_song/size", m_songsize->currentText ());
}

SettingsTabPlaylist::SettingsTabPlaylist (QWidget *parent) : QWidget (parent)
{
	QSettings s;

	s.beginGroup("playlist");
	QWidget *dummy = new QWidget (this);

	QVBoxLayout *vbox = new QVBoxLayout (dummy);
	QWidget *c = new QWidget (dummy);
	QHBoxLayout *h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	m_fontsize = new QSpinBox (c);
	m_fontsize->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	m_fontsize->setMinimum (6);
	m_fontsize->setMaximum (20);
	m_fontsize->setValue (s.value("fontsize").toInt ());
	h->addWidget (m_fontsize);

	QLabel *l = new QLabel (tr ("Playlist fontsize"), c);
	h->addWidget (l, 1);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	m_shadesize = new QSpinBox (c);
	m_shadesize->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	m_shadesize->setMinimum (6);
	m_shadesize->setMaximum (20);
	m_shadesize->setValue (s.value("shadedsize").toInt ());
	h->addWidget (m_shadesize);

	l = new QLabel (tr ("Playlist shaded mode fontsize"), c);
	h->addWidget (l, 1);
	
	s.endGroup ();
}

void
SettingsTabPlaylist::saveSettings (void)
{
	QSettings s;
	s.setValue ("playlist/fontsize", m_fontsize->value ());
	s.setValue ("playlist/shadedsize", m_shadesize->value ());
}

SettingsTabMain::SettingsTabMain (QWidget *parent) : QWidget (parent)
{
	QSettings s;

	QWidget *dummy = new QWidget (this);

	QVBoxLayout *vbox = new QVBoxLayout (dummy);
	QWidget *c = new QWidget (dummy);
	QHBoxLayout *h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	m_quitonclose = new QCheckBox (tr ("Quit XMMS2D when closing Promoe"), c);
	if (s.contains ("promoe/quitonclose"))
		s.setValue ("promoe/quitonclose", false);
	m_quitonclose->setCheckState (s.value ("promoe/quitonclose").toBool () ? Qt::Checked : Qt::Unchecked);
	h->addWidget (m_quitonclose);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	QLabel *l = new QLabel (tr ("Unshaded view"), c);
	
	QFrame *f = new QFrame (c);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	h->addWidget (l);
	h->addWidget (f, 1);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	s.beginGroup ("display_main");
	m_mainscroll = new QCheckBox (tr ("Scroll titlebar"), c);

	if (s.value("scroll").toBool ()) 
		m_mainscroll->setCheckState (Qt::Checked);
	else
		m_mainscroll->setCheckState (Qt::Unchecked);

	h->addWidget (m_mainscroll);

	m_mainsize = new QSpinBox (c);
	m_mainsize->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	m_mainsize->setMinimum (6);
	m_mainsize->setMaximum (20);
	m_mainsize->setValue (s.value("fontsize").toInt ());
	h->addWidget (m_mainsize);

	l = new QLabel (tr ("Titlebar fontsize"), c);
	l->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	h->addWidget (l);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	m_mainttf = new QCheckBox (tr ("Draw text with TrueType fonts"), c);
	m_mainttf->setCheckState (s.value ("ttf").toBool () ? Qt::Checked : Qt::Unchecked);
	h->addWidget (m_mainttf);

	
	s.endGroup ();


	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	l = new QLabel (tr ("Shaded view"), c);
	
	f = new QFrame (c);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	h->addWidget (l);
	h->addWidget (f, 1);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	s.beginGroup ("display_shaded");
	m_shadescroll = new QCheckBox (tr ("Scroll titlebar"), c);

	if (s.value("scroll").toBool ()) 
		m_shadescroll->setCheckState (Qt::Checked);
	else
		m_shadescroll->setCheckState (Qt::Unchecked);

	h->addWidget (m_shadescroll);

	m_shadesize = new QSpinBox (c);
	m_shadesize->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	m_shadesize->setMinimum (6);
	m_shadesize->setMaximum (20);
	m_shadesize->setValue (s.value("fontsize").toInt ());
	h->addWidget (m_shadesize);

	l = new QLabel (tr ("Titlebar fontsize"), c);
	l->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));
	h->addWidget (l);

	c = new QWidget (dummy);
	h = new QHBoxLayout (c);

	vbox->addWidget (c, 1);

	m_shadettf = new QCheckBox (tr ("Draw text with TrueType fonts"), c);
	m_shadettf->setCheckState (s.value ("ttf").toBool () ? Qt::Checked : Qt::Unchecked);
	h->addWidget (m_shadettf);

	/*

	QFrame *f = new QFrame (dummy);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	vbox->addWidget (f, 1);

	c = new QWidget (this);
	h = new QHBoxLayout (c);

	vbox->addWidget (c);

	m_mainttf = new QCheckBox (tr ("Draw main title with TrueType fonts"), this);
	m_mainttf->setCheckState (s.value ("mainttf").toBool () ? Qt::Checked : Qt::Unchecked);
	h->addWidget (m_mainttf);

	c = new QWidget (this);
	h = new QHBoxLayout (c);

	vbox->addWidget (c);

	m_shadettf = new QCheckBox (tr ("Draw shaded title with TrueType fonts"), this);
	m_shadettf->setCheckState (s.value ("shadettf").toBool () ? Qt::Checked : Qt::Unchecked);
	h->addWidget (m_shadettf);

	f = new QFrame (this);
	f->setFrameStyle (QFrame::HLine | QFrame::Raised);
	vbox->addWidget (f, 1);

	c = new QWidget (this);
	h = new QHBoxLayout (c);
	
	vbox->addWidget (c);

	
	
	s.endGroup ();
	*/

}

void
SettingsTabMain::saveSettings (void)
{
	QSettings s;
	
	s.setValue ("promoe/quitonclose", m_quitonclose->checkState () == Qt::Checked);

	s.beginGroup ("display_main");
	s.setValue ("scroll", m_mainscroll->checkState () == Qt::Checked);
	s.setValue ("fontsize", m_mainsize->value ());
	s.setValue ("ttf", m_mainttf->checkState () == Qt::Checked);
	s.endGroup ();

	s.beginGroup ("display_shaded");
	s.setValue ("scroll", m_shadescroll->checkState () == Qt::Checked);
	s.setValue ("fontsize", m_shadesize->value ());
	s.setValue ("ttf", m_shadettf->checkState () == Qt::Checked);
	s.endGroup ();
}

#include "XMMSHandler.h"
#include "Medialib.h"

#include <QUrl>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QSettings>
#include "qtmd5.h"

#include "MediaArtistList.h"
#include "MediaAlbumList.h"
#include "MediaSongList.h"

MedialibWindow::MedialibWindow (QWidget *parent) : QMainWindow (parent)
{
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif
	setWindowTitle ("Promoe - Medialib Window");
	setWindowFlags (Qt::Dialog);
	setAttribute (Qt::WA_DeleteOnClose);

	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	QSettings s;
	s.beginGroup ("medialib");

	if (!s.contains ("selected")) {
		s.setValue ("selected", tr ("Artists"));
	}
	
	resize (500, 550);

	m_dummy = new QWidget (this);
	setCentralWidget (m_dummy);

	m_vbox = new QVBoxLayout (m_dummy);
	m_search = new QLineEdit (m_dummy);
	m_search->setFocusPolicy (Qt::StrongFocus);
	m_vbox->addWidget (m_search);

	m_tab = new QTabWidget (m_dummy);
	m_vbox->addWidget (m_tab);

	m_progress = new QProgressBar (statusBar ());
	statusBar ()->addPermanentWidget (m_progress, 1);
	m_progress->setSizePolicy (QSizePolicy (QSizePolicy::Fixed, QSizePolicy::Fixed));

	m_status = new QLabel (tr ("Idle"), statusBar ());
	m_status->setFrameStyle (QFrame::NoFrame);
	statusBar ()->addPermanentWidget (m_status, 2);


	/*** Init views ***/
	QDir dir (QDir::homePath ()+"/.xmms2/clients/generic/art/");
	if (!dir.exists()) {
		dir.mkpath (dir.path ());
	}
	m_http = new QHttp (this);
	connect (m_http, SIGNAL (requestFinished (int, bool)), this,
							 SLOT (httpDone (int, bool)));
	connect (m_http, SIGNAL (requestStarted (int)), this,
							 SLOT (httpStarted (int)));

	/*** artist ***/
	m_artist = new MediaArtistList (this, "artist");
	m_tab->addTab (m_artist, tr ("Artists"));
	connect (m_search, SIGNAL (textEdited (QString)), 
			 m_artist, SLOT (search (QString)));

	/*** album ***/
	m_album = new MediaAlbumList (this, "album");
	m_tab->addTab (m_album, tr ("Albums"));
	connect (m_search, SIGNAL (textEdited (QString)), 
			 m_album, SLOT (search (QString)));

	/*** song ***/
	m_song = new MediaSongList (this, "song");
	m_tab->addTab (m_song, tr ("Songs"));
	connect (m_search, SIGNAL (textEdited (QString)), 
			 m_song, SLOT (search (QString)));


	/*** set active view from config ***/
	for (int i = 0; i < m_tab->count() ; i++) {
		if (s.value("selected").toString () == m_tab->tabText (i)) {
			m_tab->setCurrentIndex (i);
			break;
		}
	}

	s.endGroup ();

	setBusy (true);
	setStatusText ("Loading medialib");

	connect (xmmsh, SIGNAL (settingsSaved ()), this, SLOT (settingsSaved ()));
}

void
MedialibWindow::settingsSaved ()
{
	m_artist->setSizes();
	m_album->setSizes();
	m_song->setSizes();

	setBusy (true);
	setStatusText ("Refreshing medialib");

	m_artist->refresh();
	m_album->refresh();
	m_song->refresh();
	
	update ();
}

void
MedialibWindow::addRequest (QUrl url, MedialibListItem *item)
{
	m_http->setHost (url.host (), url.port () != -1 ? url.port () : 80);

	if (!url.userName().isEmpty()) {
		m_http->setUser (url.userName(), url.password());
	}

	int id = m_http->get (url.path (), item->getFile ());
	m_httpmap.insert (id, item);

}

void
MedialibWindow::httpStarted (int id)
{
	MedialibListItem *it = m_httpmap[id];

	if (it) {
		setStatusText ("Downloading art: " + it->text ());
	}
}

void
MedialibWindow::httpDone (int id, bool error)
{
	MedialibListItem *it = m_httpmap[id];

	if (error) {
		if (it) {
			setStatusText ("Error when downloading " + it->text ());
		} else {
			setStatusText ("Generic error in HTTP");
		}
		qDebug (qPrintable (m_http->errorString ()));
		return;
	}

	if (it) {
		QFile *f = it->getFile ();
		f->close ();

		QString newname (QDir::homePath()+"/.xmms2/clients/generic/art/"+(f->fileName ().section("/", -1)));
		f->rename (newname);
		f->setFileName (newname);

		QIcon ico (f->fileName ());
		if (!ico.isNull()) {
			it->setIcon (ico);
		} else {
			f->remove ();
		}

		delete f;
		m_httpmap.remove (id);
		if (m_httpmap.count () == 0) {
			setBusy (false);
			setStatusText ("idle");
		} else {
			setBusy (m_httpmap.count ());
			setStatusText ("Got art for: " + it->text());
		}
	}

	update ();

}



MedialibList::MedialibList (QWidget *parent, const QString &name) : QListWidget ()
{
	m_win = dynamic_cast<MedialibWindow*> (parent);

	setDragEnabled (true);

	m_name = name;
	setSizes ();
}

void
MedialibList::setSizes (void)
{
	QSettings s;

	s.beginGroup ("medialib_"+m_name);

	if (!s.contains ("size")) {
		s.setValue ("size", tr ("Large"));
	}
	
	QString size = s.value("size").toString ();
	
	m_noicon = false;

	if (size == tr ("Small")) {
		setIconSize (QSize (42, 42));
		m_font.setPixelSize (10);
	} else if (size == tr ("Large")) {
		setIconSize (QSize (85, 85));
		m_font.setPixelSize (14);
	} else if (size == tr ("None")) {
		setIconSize (QSize (10, 10));
		m_noicon = true;
		m_font.setPixelSize (10);
	}

}

void
MedialibList::search (const QString &s)
{
	if (s.length () > 0) {
		for (int i = 0; i < count (); i++) {
			MedialibListItem *it = dynamic_cast<MedialibListItem*> (item (i));
			if (!it->text().contains (s, Qt::CaseInsensitive)) {
				setItemHidden (it, true);
			} else if (isItemHidden (it)) {
				setItemHidden (it, false);
			}
		}

	} else {
		for (int i = 0; i < count (); i++) {
			MedialibListItem *it = dynamic_cast<MedialibListItem*> (item (i));
			setItemHidden (it, false);
		}
	}

}



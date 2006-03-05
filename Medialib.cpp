#include "XMMSHandler.h"
#include "Medialib.h"

#include <QUrl>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include <QSettings>
#include "qtmd5.h"

MedialibWindow::MedialibWindow (QWidget *parent) : QMainWindow (parent)
{
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif
	setWindowTitle ("Promoe - Medialib Window");

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

	m_list = new MedialibList (this);
	m_tab->addTab (new QWidget (), tr ("Artists"));
	m_tab->addTab (m_list, tr ("Albums"));
	m_tab->addTab (new QWidget (), tr ("Songs"));

	for (int i = 0; i < m_tab->count() ; i++) {
		if (s.value("selected").toString () == m_tab->tabText (i)) {
			m_tab->setCurrentIndex (i);
			break;
		}
	}

	s.endGroup ();

	connect (m_search, SIGNAL (textEdited (QString)), m_list, SLOT (search (QString)));
}

MedialibList::MedialibList (QWidget *parent) : QListWidget ()
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();
	m_http = new QHttp (this);
	m_httpmap = new QHash<int, MedialibListItem *>;
	m_win = dynamic_cast<MedialibWindow*> (parent);

	setIconSize (QSize (85, 85));
	setDragEnabled (true);

	QDir dir (QDir::homePath ()+"/.xmms2/clients/generic/art/");
	if (!dir.exists()) {
		dir.mkpath (dir.path ());
	}

	xmmsh->medialibQuery ("select distinct ifnull(nullif(ifnull(nullif(ifnull(m3.value,0),1),'Various Artists'),0),m1.value) as artist, ifnull(m2.value,'[unknown]') as album, m4.value as image from Media m1 left join Media m2 on m1.id = m2.id and m2.key='album' left join Media m3 on m1.id = m3.id and m3.key='compilation' left join Media m4 on m4.id = m1.id and m4.key='album_front_small' where m1.key='artist' order by artist, album");

	connect (xmmsh, SIGNAL (medialibResponse (QList<QHash<QString, QString> >)), 
			 this, SLOT (queryCallback (QList<QHash<QString, QString> >)));

	connect (m_http, SIGNAL (requestFinished (int, bool)), this,
							 SLOT (httpDone (int, bool)));

	m_win->setBusy (true);
	m_win->setStatusText ("Loading album view");
}

void
MedialibList::search (QString s)
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

void
MedialibList::httpDone (int id, bool error)
{
	if (error) {
		qWarning ("error!");
		return;
	}

	MedialibListItem *it = m_httpmap->value (id);

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
		m_httpmap->remove (id);
		if (m_httpmap->count () == 0) {
			m_win->setBusy (false);
			m_win->setStatusText ("idle");
		} else {
			m_win->setBusy (m_httpmap->count ());
			m_win->setStatusText ("Got art for: " + it->text());
		}
	}

	update ();

}

QStringList 
MedialibList::mimeTypes () const
{
	QStringList types;
	types << "application/mlib.album";
	return types;
}

QMimeData *
MedialibList::mimeData(const QList<QListWidgetItem*> items) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	MedialibListItem *it = dynamic_cast<MedialibListItem *> (currentItem ());
	
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	stream << it->getArtist ();
	stream << it->getAlbum ();

	mimeData->setData("application/mlib.album", encodedData);
	return mimeData;
}


void
MedialibList::queryCallback (QList<QHash<QString, QString> >l)
{
	QFont font;

	font.setPixelSize (14);

	m_win->setBusy (false);
	m_win->setStatusText ("idle");

	for (int i = 0; i < l.count (); i++) {
		QHash<QString, QString> h(l.value (i));

		MedialibListItem *item = new MedialibListItem (h.value("artist"), h.value("album"), this);
		item->setSizeHint (QSize (90, 90));
		item->setIcon (QIcon (":nocover.png"));
		item->setFont (font);
		item->setTextAlignment (Qt::AlignVCenter);

		if (h.contains ("image")) {

			QString name = qtMD5 ((h.value("artist").toLower()+"-"+h.value("album").toLower()).toUtf8());
			QString fname (QDir::homePath () +"/.xmms2/clients/generic/art/"+name+"-small.jpg");

			if (!QFile::exists (fname)) {
				QUrl url (h.value("image"));

				m_http->setHost (url.host(), url.port() != -1 ? url.port() : 80);
				if (!url.userName().isEmpty()) {
					m_http->setUser (url.userName(), url.password());
				}

				QFile *file = new QFile ("/tmp/"+name+"-small.jpg");
				file->open(QIODevice::WriteOnly);
			
				item->setFile (file);

				int id = m_http->get (url.path(), file);
				m_httpmap->insert (id, item);
			} else {
				QIcon ico (fname);
				item->setIcon (ico);
			}

		}


	}

	if (m_httpmap->count() > 0) {
		m_win->setBusy (0, m_httpmap->count ());
		m_win->setStatusText ("Loading album art");
	}

}


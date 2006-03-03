#include "XMMSHandler.h"
#include "Medialib.h"

#include <QUrl>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include "qtmd5.h"

MedialibWindow::MedialibWindow (QWidget *parent) : QMainWindow (parent)
{
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif
	
	resize (500, 550);

	m_dummy = new QWidget (parent);
	setCentralWidget (m_dummy);

	m_vbox = new QVBoxLayout (m_dummy);
	m_search = new QLineEdit (m_dummy);
	m_search->setFocusPolicy (Qt::StrongFocus);
	m_vbox->addWidget (m_search);

	m_tab = new QTabWidget (m_dummy);
	m_vbox->addWidget (m_tab);

	m_list = new MedialibList (m_tab);
	m_tab->addTab (new QWidget (m_tab), "Artists");
	m_tab->addTab (m_list, "Albums");
	m_tab->addTab (new QWidget (m_tab), "Songs");

	connect (m_search, SIGNAL (textEdited (QString)), m_list, SLOT (search (QString)));
}

MedialibList::MedialibList (QWidget *parent) : QListWidget (parent)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();
	m_http = new QHttp (this);
	m_httpmap = new QHash<int, MedialibListItem *>;

	setIconSize (QSize (85, 85));

	QDir dir (QDir::homePath ()+"/.xmms2/clients/generic/art/");
	if (!dir.exists()) {
		dir.mkpath (dir.path ());
	}

	xmmsh->medialibQuery ("select distinct m1.value as artist, ifnull(m2.value,'[unknown]') as album, m4.value as image from Media m1 left join Media m2 on m1.id = m2.id and m2.key='album' left join Media m3 on m1.id = m3.id and m3.key='compilation' left join Media m4 on m4.id = m1.id and m4.key='album_front_small' where m1.key='artist' and m3.value is null");

	connect (xmmsh, SIGNAL (medialibResponse (QList<QHash<QString, QString> >)), 
			 this, SLOT (queryCallback (QList<QHash<QString, QString> >)));

	connect (m_http, SIGNAL (requestFinished (int, bool)), this,
							 SLOT (httpDone (int, bool)));

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
		qDebug ("new path %s", qPrintable (newname));
		f->rename (newname);
		f->setFileName (newname);

		QIcon ico (f->fileName ());
		if (!ico.isNull()) {
			it->setIcon (ico);
		} else {
			qDebug ("removing %s", qPrintable (f->fileName()));
			f->remove ();
		}

		delete f;
		m_httpmap->remove (id);
	}

	update ();

}

void
MedialibList::queryCallback (QList<QHash<QString, QString> >l)
{
	QFont font;

	font.setPixelSize (14);

	for (int i = 0; i < l.count (); i++) {
		QHash<QString, QString> h(l.value (i));

		MedialibListItem *item = new MedialibListItem (h.value("artist") + " - " + h.value("album"), this);
		item->setSizeHint (QSize (90, 90));
		item->setIcon (QIcon (":nocover.jpg"));
		item->setFont (font);
		item->setTextAlignment (Qt::AlignHCenter | Qt::AlignVCenter);

		if (h.contains ("image")) {

			QString name = qtMD5 ((h.value("artist").toLower()+"-"+h.value("album").toLower()).toUtf8());
			QString fname (QDir::homePath () +"/.xmms2/clients/generic/art/"+name+".jpg");

			if (!QFile::exists (fname)) {
				QUrl url (h.value("image"));

				m_http->setHost (url.host(), url.port() != -1 ? url.port() : 80);
				if (!url.userName().isEmpty()) {
					m_http->setUser (url.userName(), url.password());
				}

				QFile *file = new QFile ("/tmp/"+name+".jpg");
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

}


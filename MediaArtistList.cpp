#include "XMMSHandler.h"

#include "MediaArtistList.h"

#include "qtmd5.h"

#include <QDir>

MediaArtistList::MediaArtistList (QWidget *parent, const QString &name) : 
	MedialibList (parent, name)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	connect (xmmsh, SIGNAL (medialibResponse (uint, QList<QHash<QString, QString> >)), 
			 this, SLOT (queryCallback (uint, QList<QHash<QString, QString> >)));

	refresh ();
}

void
MediaArtistList::refresh (void)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	m_cid = xmmsh->medialibQuery ("select distinct ifnull(nullif(ifnull(nullif(ifnull(m2.value,0),1),'Various Artists'),0),m1.value) as artist, m3.value as art from Media m1 left join Media m2 on m1.id = m2.id and m2.key='compilation' left join Media m3 on m3.id = m1.id and m3.key='artist_art_small' where m1.key='artist' order by artist");
}

QStringList 
MediaArtistList::mimeTypes () const
{
	QStringList types;
	types << "application/mlib.artist";
	return types;
}

QMimeData *
MediaArtistList::mimeData(const QList<QListWidgetItem*> items) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	MedialibListItem *it = dynamic_cast<MedialibListItem *> (currentItem ());
	
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	stream << it->getArtist ();

	mimeData->setData("application/mlib.artist", encodedData);
	return mimeData;
}


void
MediaArtistList::queryCallback (uint id, QList<QHash<QString, QString> >l)
{
	if (m_cid != id)
		return;

	clear ();

	qDebug ("in query callback artist");

	for (int i = 0; i < l.count (); i++) {
		QHash<QString, QString> h(l.value (i));

		MedialibListItem *item = new MedialibListItem (h.value("artist"), this);
		item->setFont (m_font);
		item->setTextAlignment (Qt::AlignVCenter);

		if (m_noicon) {
			continue;
		}

		item->setIcon (QIcon (":noartist.png"));
		item->setSizeHint (iconSize () + QSize (5, 5));

		if (h.contains ("art")) {

			QString name = qtMD5 ((h.value("artist").toLower()).toUtf8());
			QString fname (QDir::homePath () +"/.xmms2/clients/generic/art/"+name+"-small.jpg");

			if (!QFile::exists (fname)) {
				QUrl url (h.value("art"));

				QFile *file = new QFile ("/tmp/"+name+"-small.jpg");
				file->open(QIODevice::WriteOnly);
			
				item->setFile (file);
				m_win->addRequest (url, item);

			} else {
				QIcon ico (fname);
				item->setIcon (ico);
			}

		}


	}


}


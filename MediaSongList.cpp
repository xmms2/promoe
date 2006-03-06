#include "XMMSHandler.h"

#include "MediaSongList.h"

#include "qtmd5.h"

#include <QDir>

MediaSongList::MediaSongList (QWidget *parent, const QString &name) : 
	MedialibList (parent, name)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	connect (xmmsh, SIGNAL (medialibResponse (uint, QList<QHash<QString, QString> >)), 
			 this, SLOT (queryCallback (uint, QList<QHash<QString, QString> >)));

	refresh ();
}

void
MediaSongList::refresh (void)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	m_cid = xmmsh->medialibQuery ("select distinct ifnull(m1.value,'[unknown]') as artist, ifnull(m2.value,'[unknown]') as album, ifnull(m5.value,m6.value) as song, ifnull(m7.value,-1) as tracknr, m4.value as art from Media m1 left join Media m2 on m1.id = m2.id and m2.key='album' left join Media m4 on m4.id = m1.id and m4.key='album_front_small' left join Media m5 on m1.id = m5.id and m5.key='title' left join Media m6 on m1.id = m6.id and m6.key='url' left join Media m7 on m1.id = m7.id and m7.key='tracknr' where m1.key='artist' order by artist, album, tracknr, song");

}

QStringList 
MediaSongList::mimeTypes () const
{
	QStringList types;
	types << "application/mlib.song";
	return types;
}

QMimeData *
MediaSongList::mimeData(const QList<QListWidgetItem*> items) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	MedialibListItem *it = dynamic_cast<MedialibListItem *> (currentItem ());
	
	QDataStream stream(&encodedData, QIODevice::WriteOnly);
	stream << it->getArtist ();
	stream << it->getAlbum ();
	stream << it->getSong ();

	mimeData->setData("application/mlib.song", encodedData);
	return mimeData;
}


void
MediaSongList::queryCallback (uint id, QList<QHash<QString, QString> >l)
{
	if (m_cid != id)
		return;

	qDebug ("in query callback song");

	clear ();

	m_win->setBusy (false);
	m_win->setStatusText ("idle");

	for (int i = 0; i < l.count (); i++) {
		QHash<QString, QString> h(l.value (i));

		MedialibListItem *item = new MedialibListItem (h.value("artist"), 
													   h.value("album"), 
													   h.value("song"),
													   this);
		item->setFont (m_font);
		item->setTextAlignment (Qt::AlignVCenter);

		if (m_noicon) {
			continue;
		}

		item->setIcon (QIcon (":nosong.png"));
		item->setSizeHint (iconSize () + QSize (2, 2));

		if (h.contains ("art")) {

			QString name = qtMD5 ((h.value("artist").toLower()+"-"+h.value("album").toLower()).toUtf8());
			QString fname (QDir::homePath () +"/.xmms2/clients/generic/art/"+name+"-small.jpg");

			if (QFile::exists (fname)) {
				QIcon ico (fname);
				item->setIcon (ico);
			}

		}


	}
}


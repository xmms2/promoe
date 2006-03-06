#ifndef __MEDIAALBUMLIST_H__
#define __MEDIAALBUMLIST_H__

class MediaAlbumList;

#include "Medialib.h"

class MediaAlbumList : public MedialibList
{
	Q_OBJECT
	public:
		MediaAlbumList (QWidget *parent, const QString &name);
		~MediaAlbumList () {}
		QStringList mimeTypes (void) const;

		void refresh (void);

	public slots:
		void queryCallback (uint, QList<QHash<QString, QString> >);
		QMimeData *mimeData(const QList<QListWidgetItem*> items) const;

};

#endif

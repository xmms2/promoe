#ifndef __MEDIAARTISTLIST_H__
#define __MEDIAARTISTLIST_H__

class MediaArtistList;

#include "Medialib.h"

class MediaArtistList : public MedialibList
{
	Q_OBJECT

	public:
		MediaArtistList (QWidget *parent, const QString &name);
		~MediaArtistList () {}
		QStringList mimeTypes (void) const;
		void refresh (void);

	public slots:
		void queryCallback (uint, QList<QHash<QString, QString> >);
		QMimeData *mimeData(const QList<QListWidgetItem*> items) const;

};

#endif

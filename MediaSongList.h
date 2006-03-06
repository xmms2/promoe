#ifndef __MEDIASONGLIST_H__
#define __MEDIASONGLIST_H__

class MediaSongList;

#include "Medialib.h"

class MediaSongList : public MedialibList
{
	Q_OBJECT
	public:
		MediaSongList (QWidget *parent, const QString &name);
		~MediaSongList () {}
		QStringList mimeTypes (void) const;
		void refresh (void);

	public slots:
		void queryCallback (uint, QList<QHash<QString, QString> >);
		QMimeData *mimeData(const QList<QListWidgetItem*> items) const;

};

#endif

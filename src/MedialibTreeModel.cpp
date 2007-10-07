#include <xmmsclient/xmmsclient++.h>
#include "XmmsQt4.h"
#include "MedialibTreeModel.h"
#include "MedialibItem.h"

#include <QAbstractItemModel>
#include <QErrorMessage>
#include <QString>

MedialibTreeModel::MedialibTreeModel (QObject *parent) :
	QAbstractItemModel (parent), m_client ("PromoeMedialibTree")
{
	const char *path = getenv("XMMS_PATH");
	try {
		m_client.connect (path ? path : "");
	}
	catch (Xmms::connection_error& e) {
		QErrorMessage *err = new QErrorMessage ();
		err->showMessage ("Couldn't connect to XMMS2, please try again.");
		err->exec ();
		delete err;
	}

	m_client.setMainloop (new XmmsQT4 (m_client.getConnection ()));

	m_rootitem = new MedialibItem ();

	using Xmms::bind;

	m_client.medialib.select ("select distinct value from Media where key='artist' order by lower(value)",
							  bind (&MedialibTreeModel::artist_list, this));

}

bool
MedialibTreeModel::artist_list (const Xmms::List< Xmms::Dict > &artists)
{
	QString s;

	for (artists.first (); artists.isValid (); ++artists) {
		s = QString::fromUtf8 ((*artists).get<std::string> ("value").c_str ());
		new MedialibItem (QStringList (s), m_rootitem, MedialibItem::ArtistItem);
	}

	reset ();

	return true;
}

/* QModel overrides */

Qt::ItemFlags
MedialibTreeModel::flags (const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int
MedialibTreeModel::rowCount (const QModelIndex &parent) const
{
	MedialibItem *item;

	if (!parent.isValid ()) {
		item = m_rootitem;
	} else {
		item = static_cast<MedialibItem*> (parent.internalPointer ());
	}

	int n = item->childCount ();

	if (n == 0 && item->mayHaveChilds ())
		n = 1;

	return n;
}

QModelIndex
MedialibTreeModel::parent (const QModelIndex &index) const
{
	if (!index.isValid ())
		return QModelIndex ();

	MedialibItem *item = static_cast<MedialibItem*> (index.internalPointer ());
	MedialibItem *parent = item->parent ();

	if (parent == m_rootitem) {
		return QModelIndex ();
	}

	return createIndex (parent->row (), 0, parent);
}

QModelIndex
MedialibTreeModel::index (int row, int column,
						  const QModelIndex &parent) const
{
	MedialibItem *p;
	if (!parent.isValid ()) {
		p = m_rootitem;
	} else {
		p = static_cast<MedialibItem*> (parent.internalPointer ());
	}
	
	MedialibItem *child = p->child (row);
	if (child) {
		return createIndex (row, column, child);
	} else {
		if (p->columnCount () == 0 && !p->loading ()) {
			qDebug ("Loading items...");
			switch (p->type ()) {
				case MedialibItem::ArtistItem:
					m_client.medialib.select ("select distinct m1.value as value from Media m1 join Media m2 on m1.id = m2.id and m2.key = 'artist' where m1.key='album' and m2.value = '%s' order by lower(m1.value)", MedialibTreeModel::sub_list);

	return QModelIndex ();
}

int
MedialibTreeModel::columnCount (const QModelIndex &parent) const
{
	return 1;
}

QVariant
MedialibTreeModel::data (const QModelIndex &index,
						 int role) const
{
	if (!index.isValid ())
		return QVariant ();

	if (role != Qt::DisplayRole)
		return QVariant ();

	MedialibItem *item = static_cast<MedialibItem *> (index.internalPointer ());
	return QVariant (item->data (index.column ()));
}

QVariant
MedialibTreeModel::headerData (int section,
							   Qt::Orientation orientation,
							   int role) const
{
	if (role == Qt::DisplayRole)
		return QVariant("Title");
	return QVariant ();
}


#ifndef __MEDIALIBTREEMODEL_H__
#define __MEDIALIBTREEMODEL_H__

#include <xmmsclient/xmmsclient++.h>

#include "MedialibItem.h"

#include <QAbstractItemModel>
#include <QStringList>

class MedialibTreeModel : public QAbstractItemModel
{
	Q_OBJECT
	public:
		MedialibTreeModel (QObject *parent);
		MedialibTreeModel::~MedialibTreeModel () {};

		/* QModel overrides */
		int rowCount (const QModelIndex &parent) const;
		int columnCount (const QModelIndex &parent) const;
		QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags (const QModelIndex &index) const;
		QModelIndex index (int row, int column, const QModelIndex &parent) const;
		QModelIndex parent (const QModelIndex &index) const;

	private:
		bool artist_list (const Xmms::List< Xmms::Dict > &artists);
		Xmms::Client m_client;
		MedialibItem *m_rootitem;
};

#endif

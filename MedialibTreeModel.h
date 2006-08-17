#ifndef __MEDIALIBTREEMODEL_H__
#define __MEDIALIBTREEMODEL_H__

#include "XMMSSocket.h"
#include <QAbstractTableModel>

class MedialibTreeModel : public QAbstractTableModel
{
	Q_OBJECT
	public:
		MedialibTreeModel (QObject *parent);
		MedialibTreeModel::~MedialibTreeModel () {};

		/* QModel overrides */
		int rowCount (const QModelIndex &parent) const;
		int columnCount (const QModelIndex &parent) const;
		QVariant data (const QModelIndex &parent, int role = Qt::DisplayRole) const;
		QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	private:
		XMMSSocket m_socket;
};

#endif

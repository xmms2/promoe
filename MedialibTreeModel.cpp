#include "XMMSSocket.h"
#include "MedialibTreeModel.h"

#include <QAbstractTableModel>

MedialibTreeModel::MedialibTreeModel (QObject *parent) : QAbstractTableModel (parent), m_socket ("PromoeMedialibTree")
{
	m_socket.connect (getenv ("XMMS_PATH"));
}

/* QModel overrides */
int
MedialibTreeModel::rowCount (const QModelIndex &parent) const
{
	return 2;
}

int
MedialibTreeModel::columnCount (const QModelIndex &parent) const
{
	return 2;
}

QVariant
MedialibTreeModel::data (const QModelIndex &parent,
						 int role) const
{
	if (role == Qt::DisplayRole)
		return QVariant("hej");
	return QVariant ();
}

QVariant
MedialibTreeModel::headerData (int section,
							   Qt::Orientation orientation,
							   int role) const
{
	if (role == Qt::DisplayRole)
		return QVariant("header");
	return QVariant ();
}


#include "MedialibTreeModel.h"
#include "MedialibView.h"
#include <QTreeView>


MedialibView::MedialibView (QWidget *parent) : QTreeView (parent)
{
	setAlternatingRowColors (true);
	setModel (new MedialibTreeModel (this));
}

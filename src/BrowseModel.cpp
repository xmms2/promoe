#include "XMMSHandler.h"
#include "BrowseModel.h"

#include <QAbstractTableModel>
#include <QString>
#include <QIcon>
#include <QWidget>


bool
BrowseModelItem::itemCompare (BrowseModelItem *s1, BrowseModelItem *s2)
{
	if (QString::localeAwareCompare (s1->m_vals["name"].toLower (),
									 s2->m_vals["name"].toLower ()) < 0) {
		return true;
	}
	return false;
}

BrowseModel::BrowseModel (QWidget *parent) : QAbstractTableModel ()
{
	m_columns.append ("Name");
	m_style = parent->style ();
	m_client = XMMSHandler::getInstance ().getClient ();
	m_filter_dot = true;
	//list_root ();
}

BrowseModelItem *
BrowseModel::itemByIndex (const QModelIndex &index)
{
	return m_list.at (index.row ());
}

void
BrowseModel::setPath (const QModelIndex &index)
{
	BrowseModelItem *item = m_list.at (index.row ());
	m_client->xform.browse (item->data("path").toStdString ()) (Xmms::bind (&BrowseModel::list_cb, this));
	m_current_dir = item->data ("path");
}

void
BrowseModel::setPath (const QString &path)
{
	if (path.isEmpty())
		list_root ();

	m_client->xform.browse (path.toStdString ())(
							Xmms::bind (&BrowseModel::list_cb, this),
							Xmms::bind (&BrowseModel::list_err, this));
	m_current_dir = path;
}

void
BrowseModel::list_root ()
{
	while (!m_list.isEmpty ()) {
		delete m_list.takeFirst ();
	}

	m_list.append (new BrowseModelItem ("file:///", "Files", true));
	m_list.append (new BrowseModelItem ("daap://", "DAAP", true));
	emit dirChanged ("");

	reset ();

	m_current_dir = "";
}

bool
BrowseModel::list_err (const std::string err)
{
	list_root ();
	return true;
}

bool
BrowseModel::list_cb (const Xmms::List< Xmms::Dict > &res)
{
	while (!m_list.isEmpty ()) {
		delete m_list.takeFirst ();
	}

	for (res.first (); res.isValid (); ++res) {
		Xmms::Dict d = *res;

		if (!d.contains ("path"))
			continue;

		QString path = QString::fromStdString (d.get<std::string> ("path"));

		QString name;
		if (d.contains ("name")) {
			name = QString::fromStdString (d.get<std::string> ("name"));
		} else {
			if (d.contains ("title")) {
				if (d.contains ("artist")) {
					name += QString::fromStdString (d.get<std::string> ("artist"));
					name += " - ";
				}
				if (d.contains ("album")) {
					name += QString::fromStdString (d.get<std::string> ("album"));
					name += " - ";
				}
				if (d.contains ("tracknr")) {
					name += QString::number (d.get<uint32_t>
											 ("tracknr")).rightJustified(2, '0');
					name += " - ";
				}
				name += QString::fromStdString (d.get<std::string> ("title"));
			} else {
				const char *tmp;
				QString tmp2 = path.mid (path.lastIndexOf ("/")+1);
				tmp = xmmsc_result_decode_url (NULL, tmp2.toAscii ());
				name = QString::fromUtf8 (tmp);
				free ((char *)tmp);
			}
		}

		bool isdir = d.get<int32_t> ("isdir");

		if (m_filter_dot && name.startsWith ("."))
			// skip these files 
			continue;

		m_list.append (new BrowseModelItem (path, name, isdir));
	}

	qSort (m_list.begin (), m_list.end (), BrowseModelItem::itemCompare);

	qDebug ("%s", qPrintable(m_list.at(0)->data("name")));

	reset ();

	emit dirChanged (m_current_dir);

	return true;
}

/* QModel overrides */
int
BrowseModel::rowCount (const QModelIndex &parent) const
{
	return m_list.size ();
}

int
BrowseModel::columnCount (const QModelIndex &parent) const
{
	return m_columns.size ();
}

QVariant
BrowseModel::data (const QModelIndex &index,
						 int role) const
{
	if (!index.isValid ())
		return QVariant ();

	if (index.column () == 0 && role == Qt::DecorationRole)
		return fileIcon(index);

	if (role != Qt::DisplayRole)
		return QVariant ();

	QString h = m_columns[index.column ()].toLower ();

	return QVariant (m_list.at (index.row ())->data (h));
}

QVariant
BrowseModel::headerData (int section,
						 Qt::Orientation orientation,
						 int role) const
{
	if (role == Qt::DisplayRole)
		return QVariant (m_columns[section]);
	return QVariant ();
}

QIcon
BrowseModel::fileIcon (const QModelIndex &index) const
{
    if (!index.isValid())
		return QIcon ();

	BrowseModelItem *item = m_list.at (index.row ());
	if (item && item->isDir ())
    	return QIcon (m_style->standardPixmap (QStyle::SP_DirClosedIcon));

	return QIcon (m_style->standardPixmap (QStyle::SP_FileIcon));
}

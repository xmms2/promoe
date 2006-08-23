#ifndef __BROWSEMODEL_H__
#define __BROWSEMODEL_H__

#include <xmmsclient/xmmsclient++.h>

#include <QHash>
#include <QString>
#include <QStyle>
#include <QStringList>
#include <QAbstractTableModel>

class BrowseModelItem
{
	public:
		BrowseModelItem (const QString &path,
						 const QString &name = 0,
						 bool isdir = true)
		{
			m_isdir = isdir;
			m_vals["path"] = path;
			m_vals["name"] = name;
		};

		QString data (const QString &key) {
			return m_vals[key];
		};

		bool isDir () {
			return m_isdir;
		};

	protected:
		QHash<QString, QString> m_vals;
		bool m_isdir;

};

class BrowseModel : public QAbstractTableModel
{
	Q_OBJECT
	public:
		BrowseModel (QWidget *parent);

		QString currentPath () const
		{
			return m_current_dir;
		};

		void setFilterDot (bool b)
		{
			m_filter_dot = b;
		};

		void setPath (const QModelIndex &index);
		void setPath (const QString &path);
		BrowseModelItem *itemByIndex (const QModelIndex &index);

		/* QModel overrides */
		int rowCount (const QModelIndex &parent) const;
		int columnCount (const QModelIndex &parent) const;
		QVariant data (const QModelIndex &index,
					   int role = Qt::DisplayRole) const;
		QVariant headerData (int section,
							 Qt::Orientation orientation,
							 int role = Qt::DisplayRole) const;
		QIcon fileIcon (const QModelIndex &index) const;

	signals:
		void dirChanged (QString);

	private:
		bool list_cb (const Xmms::List< Xmms::Dict > &res);
		bool list_err (const std::string err);

		void list_root ();

		bool m_filter_dot;

		QList<BrowseModelItem*> m_list;
		QStringList m_columns;
		QString m_current_dir;
		QStyle *m_style;
		Xmms::Client *m_client;
};

#endif

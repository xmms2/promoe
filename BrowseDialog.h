#ifndef __BROWSEDIALOG_H__
#define __BROWSEDIALOG_H__

#include "BrowseModel.h"

#include <QDialog>
#include <QTreeView>
#include <QListView>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QItemSelectionModel>

class BrowseDialog : public QDialog
{
	Q_OBJECT
	public:
		BrowseDialog (QWidget *parent);

	public slots:
		void setPath (const QModelIndex &index);
		void setPath (const QString &path);
		void navigateToPrevious ();
		void dirChanged (const QString &path);
		void accept ();
		QStringList getFiles ();

	private:
		QTreeView *m_list;
		QComboBox *m_lookInCombo;
		QLineEdit *m_lookInEdit;
		QLineEdit *m_fileNameEdit;
		QComboBox *m_fileTypeCombo;
		BrowseModel *m_model;
		QItemSelectionModel *m_selections;
};
#endif 

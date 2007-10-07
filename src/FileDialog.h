#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

#include <QFileDialog>

class FileDialog : public QFileDialog
{
	Q_OBJECT
	public:
		FileDialog (QWidget *parent, const QString &name);
		QString getDirectory ();
		QStringList getFiles ();

	private:
		QString m_name;

};

#endif

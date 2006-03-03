#ifndef __MEDIALIB_H__
#define __MEDIALIB_H__

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHttp>
#include <QFile>
#include <QListWidgetItem>

class MedialibListItem : public QListWidgetItem
{
	public:
		MedialibListItem (QString text, QListWidget *parent) : QListWidgetItem (text, parent) {}
		~MedialibListItem () {}
		void setFile (QFile *f) { m_file = f; }
		QFile *getFile (void) { return m_file; }

	private:
		QFile *m_file;
};

class MedialibList : public QListWidget
{
	Q_OBJECT
	public:
		MedialibList (QWidget *parent);
		~MedialibList () {}

	public slots:
		void queryCallback (QList<QHash<QString, QString> >);
		void httpDone (int, bool);
		void search (QString);

	private:
		QHash<int, MedialibListItem*> *m_httpmap;
		QHttp *m_http;

};

class MedialibWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MedialibWindow (QWidget *parent);
		~MedialibWindow () {}

	
	private:
		QWidget *m_dummy;
		QTabWidget *m_tab;
		QLineEdit *m_search;
		QVBoxLayout *m_vbox;
		MedialibList *m_list;
};

#endif

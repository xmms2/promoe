#ifndef __MEDIALIB_H__
#define __MEDIALIB_H__

#include <QListWidgetItem>
#include <QProgressBar>
#include <QMainWindow>
#include <QListWidget>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QTabWidget>
#include <QMimeData>
#include <QLineEdit>
#include <QLabel>
#include <QHttp>
#include <QFile>

class MedialibWindow;

class MedialibListItem : public QListWidgetItem
{
	public:
		MedialibListItem (QString artist, QString album, QListWidget *parent) : 
			QListWidgetItem (artist + " - " + album, parent) {
				m_artist = artist;
				m_album = album;
			}
		~MedialibListItem () {}
		void setFile (QFile *f) { m_file = f; }
		QFile *getFile (void) { return m_file; }
		QString getArtist (void) { return m_artist; }
		QString getAlbum (void) { return m_album; }

	private:
		QFile *m_file;
		QString m_artist;
		QString m_album;
};

class MedialibList : public QListWidget
{
	Q_OBJECT
	public:
		MedialibList (QWidget *parent);
		~MedialibList () {}
		QStringList mimeTypes (void) const;

	public slots:
		void queryCallback (QList<QHash<QString, QString> >);
		void httpDone (int, bool);
		void search (QString);
		QMimeData *mimeData(const QList<QListWidgetItem*> items) const;

	private:
		QHash<int, MedialibListItem*> *m_httpmap;
		QHttp *m_http;
		MedialibWindow *m_win;
		int m_httpr;

};

class MedialibWindow : public QMainWindow
{
	Q_OBJECT
	public:
		MedialibWindow (QWidget *parent);
		~MedialibWindow () {}

		void setBusy (bool b) { 
			if (b) {
				m_progress->setMaximum (0);
				m_progress->setMinimum (0);
			} else {
				m_progress->setMaximum (1);
				m_progress->reset ();
			}
		}
		void setBusy (int min, int max) {
			m_progress->setMaximum (max);
			m_progress->setMinimum (min);
		}
		void setBusy (int cur) {
			m_progress->setValue (cur);
		}
		void setStatusText (QString s) {
			m_status->setText (s);
		}

	private:
		QWidget *m_dummy;
		QTabWidget *m_tab;
		QLineEdit *m_search;
		QVBoxLayout *m_vbox;
		QProgressBar *m_progress;
		QLabel *m_status;
		MedialibList *m_list;
};

#endif

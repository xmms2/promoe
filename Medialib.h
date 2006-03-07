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
#include <QUrl>
#include <QMap>

class MedialibWindow;

class MedialibListItem : public QListWidgetItem
{
	public:
		MedialibListItem (const QString &artist, const QString &album,
						  const QString &song, QListWidget *parent) : 
			QListWidgetItem (artist + " - " + song, parent) {
				m_artist = artist;
				m_album = album;
				m_song = song;
			}
		MedialibListItem (const QString &artist, const QString &album, 
						  QListWidget *parent) : 
			QListWidgetItem (artist + " - " + album, parent) {
				m_artist = artist;
				m_album = album;
			}
		MedialibListItem (const QString &artist, QListWidget *parent) : 
			QListWidgetItem (artist, parent) {
				m_artist = artist;
			}

		~MedialibListItem () {}
		void setFile (QFile *f) { m_file = f; }
		QFile *getFile (void) { return m_file; }

		QString getArtist (void) { return m_artist; }
		QString getAlbum (void) { return m_album; }
		QString getSong (void) { return m_song; }

	private:
		QFile *m_file;
		QString m_artist;
		QString m_album;
		QString m_song;
};

class MedialibList : public QListWidget
{
	Q_OBJECT
	public:
		MedialibList (QWidget *parent, const QString &name);
		~MedialibList () {}
		void setSizes (void);
		void refresh (void) {}
		/*
		QStringList mimeTypes (void) const {};
		*/

	public slots:

		/*
		void queryCallback (uint, QList<QHash<QString, QString> >) {};
		QMimeData *mimeData(const QList<QListWidgetItem*> items) const {};
		*/

		void search (const QString &);

	protected:
		uint m_cid;
		QFont m_font;
		MedialibWindow *m_win;
		bool m_noicon;

	private:
		QString m_name;

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

		void addRequest (QUrl url, MedialibListItem *item);

	public slots:
		void httpDone (int, bool);
		void settingsSaved (void);

	private:
		QWidget *m_dummy;
		QTabWidget *m_tab;
		QLineEdit *m_search;
		QVBoxLayout *m_vbox;
		QProgressBar *m_progress;
		QLabel *m_status;
		MedialibList *m_artist;
		MedialibList *m_album;
		MedialibList *m_song;

		QMap<int, MedialibListItem*> m_httpmap;
		QHttp *m_http;
};

#endif

#ifndef __SETTINGSWINDOW_H__
#define __SETTINGSWINDOW_H__

#include <QWidget>
#include <QDialog>

class QCheckBox;
class QSpinBox;
class QComboBox;


class SettingsTabMedialib : public QWidget {
	Q_OBJECT
	public:
		SettingsTabMedialib (QWidget *parent);
		~SettingsTabMedialib () {}
		void saveSettings (void);

	private:
		QComboBox *m_selected;
		QComboBox *m_artistsize;
		QComboBox *m_albumsize;
		QComboBox *m_songsize;
};

class SettingsTabPlaylist : public QWidget {
	Q_OBJECT
	public:
		SettingsTabPlaylist (QWidget *parent);
		~SettingsTabPlaylist () {}
		void saveSettings (void);

	private:
		QSpinBox *m_fontsize;
		QSpinBox *m_shadesize;
		QCheckBox *m_remote_fs;
};

class SettingsTabMain : public QWidget {
	Q_OBJECT
	public:
		SettingsTabMain (QWidget *parent);
		~SettingsTabMain () {}
		void saveSettings (void);


	private:
		QCheckBox *m_quitonclose;

		QSpinBox *m_mainsize;
		QCheckBox *m_mainttf;
		QCheckBox *m_mainscroll;
		QSpinBox *m_shadesize;
		QCheckBox *m_shadettf;
		QCheckBox *m_shadescroll;
};

class SettingsDialog : public QDialog {
	Q_OBJECT
	public:
		SettingsDialog (QWidget *parent);
		~SettingsDialog () {}

	public slots:
		void okButton (void);

	private:
		SettingsTabMain *m_mainwindow;
		SettingsTabPlaylist *m_playlistwin;
		SettingsTabMedialib *m_medialib;

};


#endif

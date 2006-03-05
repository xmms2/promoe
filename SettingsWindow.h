#ifndef __SETTINGSWINDOW_H__
#define __SETTINGSWINDOW_H__

#include <QMainWindow>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QLabel>

class SettingsTabMain : public QWidget {
	Q_OBJECT
	public:
		SettingsTabMain (QWidget *parent);
		~SettingsTabMain () {}
		void saveSettings (void);


	private:
		QCheckBox *m_scrolltbar;
		QSpinBox *m_fontsize;
		QCheckBox *m_quitonclose;
		QCheckBox *m_mainttf;
		QCheckBox *m_shadettf;
};

class SettingsWindow : public QMainWindow {
	Q_OBJECT
	public:
		SettingsWindow (QWidget *parent);
		~SettingsWindow () {}

	public slots:
		void okButton (void);

	private:
		SettingsTabMain *m_mainwindow;
		QWidget *m_playlistwin;
		QWidget *m_medialib;

};


#endif

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
		QCheckBox *m_quitonclose;

		QSpinBox *m_mainsize;
		QCheckBox *m_mainttf;
		QCheckBox *m_mainscroll;
		QSpinBox *m_shadesize;
		QCheckBox *m_shadettf;
		QCheckBox *m_shadescroll;
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

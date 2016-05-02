/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef __SETTINGSWINDOW_H__
#define __SETTINGSWINDOW_H__

#include <QWidget>
#include <QDialog>

#include <application.h>

class QCheckBox;
class QSpinBox;
class QComboBox;


class SettingsTabMedialib : public QWidget {
	Q_OBJECT
	public:
		SettingsTabMedialib (QWidget *parent);
		~SettingsTabMedialib () {}
		bool saveSettings (void);

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
		bool saveSettings (void);

	private:
		QSpinBox *m_fontsize;
		QSpinBox *m_shadesize;
		QCheckBox *m_remote_fs;
		QCheckBox *m_scroll_view;
};

class SettingsTabMain : public QWidget {
	Q_OBJECT
	public:
		SettingsTabMain (QWidget *parent);
		~SettingsTabMain () {}
		bool saveSettings (void);


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

	signals:
		void settingsChanged (void);

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

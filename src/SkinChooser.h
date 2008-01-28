/**
 *  This file is a part of Prome, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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


#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>

class MainWindow;

class SkinList : public QListWidget
{
	Q_OBJECT
	public:
		SkinList (QWidget *parent);
		~SkinList () {}

	public slots:
		void changeSkin (QListWidgetItem *item);
};

class SkinChooserItem : public QListWidgetItem
{
	public:
		SkinChooserItem (QIcon i, QString n, QString p, QListWidget *parent) :
			QListWidgetItem (i, n, parent) {
				m_path = p;
			}
		~SkinChooserItem () {}
		QString getPath (void) { return m_path; }
	private:
		QString m_path;

};

class SkinChooser : public QMainWindow
{
	public:
		SkinChooser (QWidget *parent);
		~SkinChooser (void) {}
		MainWindow *getMW (void) { return m_mw; }

	private:
		QVBoxLayout *m_vbox;
		SkinList *m_skin;
		QLabel *m_label;
		QWidget *m_c;
		MainWindow *m_mw;

};


/**
 *  This file is a part of Promoe, an XMMS2 client
 *
 *  Copyright (C) 2008 Thomas Frauendorfer
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef __PLAYLISTCHOOSER_H__
#define __PLAYLISTCHOOSER_H__

#include "ui_playlistchooser.h"

#include <QDialog>
class QListWidgetItem;

class XCollection;

class PlaylistChooser : public QDialog,  private Ui::PlaylistChooser {
	Q_OBJECT

	public:
		PlaylistChooser (QWidget *parent, XCollection *coll);


	private slots:
		void handle_playlists_modified (QString, QString, int, QString);

		void on_playlistCreateButton_clicked ();
		void on_playlistRemoveButton_clicked ();

		void on_playlistsListWidget_itemDoubleClicked (QListWidgetItem* item);

	private:
		XCollection* m_collection;
};

#endif

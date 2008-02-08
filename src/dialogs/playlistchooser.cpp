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

// FIXME: because somewhere something with the includes is wrons, this line
// is needed
#include "XMMSHandler.h"

#include <QRegExp>
#include <QStringList>
#include <QList>
#include <QListWidgetItem>

#include "playlistchooser.h"

#include "xcollection.h"

PlaylistChooser::PlaylistChooser (QWidget *parent, XCollection *coll)
                                  : QDialog (parent)
{
	setupUi (this);
	setAttribute (Qt::WA_DeleteOnClose);

	m_collection = coll;

	// FIXME: implement creation of new Playlists
//	playlistCreateButton->setEnabled (false);
//	playlistRemoveButton->setEnabled (false);

	// fill the List with Playlistnames.
	// Sort out playlists that start with '_'
	QRegExp regex = QRegExp ("^[^_]");
	QStringList lists = coll->list ("Playlists").filter (regex);
	playlistsListWidget->setSortingEnabled (true);
	playlistsListWidget->addItems (lists);
	connect (coll, SIGNAL (collectionModified (QString, QString, int, QString)),
	         this, SLOT (handle_playlists_modified (QString, QString, int,
			                                       QString)));

}

void
PlaylistChooser::handle_playlists_modified (QString name, QString ns,
                                            int type, QString newname)
{
	if (ns != "Playlists") {
		return;
	}

	switch (type) {
		case XMMS_COLLECTION_CHANGED_ADD:
			if (!name.startsWith("_")) {
				playlistsListWidget->addItem(name);
			}
			break;
		case XMMS_COLLECTION_CHANGED_REMOVE: {
				QList<QListWidgetItem *> list
				   = playlistsListWidget->findItems (name, Qt::MatchExactly);
				if (!list.empty ()) {
					// we should only have one exatly matching String
					QListWidgetItem* item = list.first ();
					int idx = playlistsListWidget->row (item);
					item = playlistsListWidget->takeItem (idx);
					delete item;
				}
			}
			break;
		case XMMS_COLLECTION_CHANGED_RENAME: {
				// remove the old entry
				QList<QListWidgetItem *> list
				   = playlistsListWidget->findItems (name, Qt::MatchExactly);
				if (!list.empty ()) {
					// we should only have one exatly matching String
					QListWidgetItem* item = list.first ();
					int idx = playlistsListWidget->row (item);
					item = playlistsListWidget->takeItem (idx);
					delete item;
				}
				// and add the new one
				if (!newname.startsWith("_")) {
					playlistsListWidget->addItem(newname);
				}
			}
			break;

		default:
			break;
	}
}


void
PlaylistChooser::on_playlistCreateButton_clicked ()
{
	QString name = playlistTextEdit->text ();
	// only create new playlist, if it doesn't already exist
	if (m_collection->list ("Playlists").contains (name)) return;

	m_collection->addIdlist (name);
}

void
PlaylistChooser::on_playlistRemoveButton_clicked ()
{
	QList<QListWidgetItem *> list = playlistsListWidget->selectedItems ();
	if (list.empty ()) return; // nothing to do

	// TODO: if we change the selectionmodel to multiselection,
	// change this tp remove more than one item
	QListWidgetItem* item = list.first ();
	QString name = item->text ();
	m_collection->remove (name, "Playlists");
}

void
PlaylistChooser::on_playlistsListWidget_itemDoubleClicked (QListWidgetItem* item)
{
	QString name = item->text ();
	m_collection->setActivePlaylist (name);
}

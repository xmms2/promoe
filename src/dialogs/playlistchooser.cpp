/**
 *  This file is a part of Promoe, an XMMS2 client
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
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

// FIXME: because somewhere something with the includes is wrong, this line
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

	// the createButton will be enabled as soon as textEdit contains a name
	// that is not equal to an existing Playlist
	createButton->setEnabled (false);

	// selectButton will be enabled when exactly one item is selected
	selectButton->setEnabled (false);

	// fill the List with Playlistnames.
	// Sort out playlists that start with '_'
	QRegExp regex = QRegExp ("^[^_]");
	QStringList lists = coll->list ("Playlists").filter (regex);
	listWidget->setSortingEnabled (true);
	listWidget->addItems (lists);
	connect (coll, SIGNAL (collectionModified (QString, QString, int, QString)),
	         this, SLOT (handle_playlists_modified (QString, QString, int,
			                                       QString)));

	// display active Playlist in bold Font
	QString activePls = coll->activePlaylist ();
	handle_active_pls_changed (activePls, "");
	connect (coll, SIGNAL (activePlaylistChanged (QString, QString)),
	         this, SLOT (handle_active_pls_changed (QString, QString)));
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
				listWidget->addItem(name);
			}
			// if we created the playlist, make is the active playlist
			if (name == textEdit->text ()) {
				m_collection->setActivePlaylist (name);
			}
			break;
		case XMMS_COLLECTION_CHANGED_REMOVE: {
				QList<QListWidgetItem *> list
				   = listWidget->findItems (name, Qt::MatchExactly);
				if (!list.empty ()) {
					// we should only have one exatly matching String
					QListWidgetItem* item = list.first ();
					int idx = listWidget->row (item);
					item = listWidget->takeItem (idx);
					delete item;
				}
			}
			break;
		case XMMS_COLLECTION_CHANGED_RENAME: {
				// remove the old entry
				QList<QListWidgetItem *> list
				   = listWidget->findItems (name, Qt::MatchExactly);
				if (!list.empty ()) {
					// we should only have one exatly matching String
					QListWidgetItem* item = list.first ();
					int idx = listWidget->row (item);
					item = listWidget->takeItem (idx);
					delete item;
				}
				// and add the new one
				if (!newname.startsWith("_")) {
					listWidget->addItem(newname);
				}
			}
			break;

		default:
			break;
	}
}


void
PlaylistChooser::handle_active_pls_changed (QString newActive,
                                            QString oldActive)
{
	QListWidgetItem *item;
	QFont font;
	QList <QListWidgetItem *> list;

	// newActive and oldActive can match at most once each
	// paint former active Playlist no longer in bold
	list = listWidget->findItems (oldActive, Qt::MatchExactly);
	if (!list.empty ()) {
		item = list.first ();
		font = item->font ();
		font.setBold (false);
		item->setFont (font);
	}

	// paint new active Playlist in bold
	list = listWidget->findItems (newActive, Qt::MatchExactly);
	if (!list.empty ()) {
		item = list.first ();
		font = item->font ();
		font.setBold (true);
		item->setFont (font);
	}

	// update the activestate of the selectButton
	on_listWidget_itemSelectionChanged ();
}


bool
PlaylistChooser::playlistExists (QString name) {
	// Use the information from m_collection, as listWidger
	// doesn't contain hidden Playlists
	return m_collection->list ("Playlists").contains (name);
}


void
PlaylistChooser::on_removeButton_clicked ()
{
	QList<QListWidgetItem *> list = listWidget->selectedItems ();
	if (list.empty ())
		return; // nothing to do

	// TODO: if we change the selectionmodel to multiselection,
	// change this tp remove more than one item
	QListWidgetItem* item = list.first ();
	QString name = item->text ();
	m_collection->remove (name, "Playlists");
}


void
PlaylistChooser::on_createButton_clicked ()
{
	QString name = textEdit->text ();
	// only create new playlist, if it doesn't already exist
	if (playlistExists (name)) return;

	m_collection->addIdlist (name);
}


void
PlaylistChooser::on_selectButton_clicked ()
{
	QList <QListWidgetItem *> list = listWidget->selectedItems ();
	if (list.size () == 1) {
		QListWidgetItem * item = list.first ();
		m_collection->setActivePlaylist (item->text ());
	}
}


void
PlaylistChooser::on_listWidget_itemDoubleClicked (QListWidgetItem* item)
{
	QString name = item->text ();
	m_collection->setActivePlaylist (name);
}

void
PlaylistChooser::on_listWidget_itemSelectionChanged ()
{
	QList <QListWidgetItem *> list = listWidget->selectedItems ();
	// enable selectButton, if one item is selected and does not represent
	// the currently active playlist
	selectButton->setEnabled ((list.size () == 1)
	                          && (list.first ()->text ()
							      != m_collection->activePlaylist ()));

	// enable removeButton if more than one item is selected
	// or if the selected item is not the actice Playlist
	removeButton->setEnabled ((list.size () > 1) ||
	                          ( (list.size () == 1) && (list.first ()->text ()
	                                    != m_collection->activePlaylist ())));
}

void
PlaylistChooser::on_textEdit_textChanged ()
{
	// Enable createButton only, if no playlist with that name exists
	// and the textEdit is not empty
	createButton->setEnabled ((!playlistExists (textEdit->text ())
	                           && (textEdit->text () != "")));
}

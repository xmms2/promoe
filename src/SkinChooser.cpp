/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
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

#include "Skin.h"
#include "SkinChooser.h"

#include <QDir>
#include <QFile>
#include <QIcon>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>

SkinChooser::SkinChooser (QWidget *parent) : QDialog (parent)
{

#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif
	setWindowModality (Qt::ApplicationModal);
	setAttribute (Qt::WA_DeleteOnClose);

	QVBoxLayout *vbox = new QVBoxLayout (this);
	QLabel *label = new QLabel ("Available skins...", this);
	label->setFont (QFont ("Helvetica", 16));
	vbox->addWidget (label);

	m_skin = new SkinList (this);
	vbox->addWidget (m_skin);

	resize (500, 300);

}

SkinList::SkinList (QWidget *parent) : QListWidget (parent)
{
	setIconSize (QSize (137, 58));

	new SkinChooserItem (QIcon (":CleanAMP/main.png"), "CleanAMP (default)",
	                     ":CleanAMP/", this);

	QSettings settings;
	QStringList searchpath;
	if (settings.contains ("skin/searchpath") ) {
		searchpath = settings.value ("skin/searchpath").toStringList ();
	} else {
		QString path;
		path.append (QDir::homePath());
		path.append ("/.config/xmms2/clients/promoe/skins/");
		searchpath.append (path);
		settings.setValue ("skin/searchpath", searchpath);
	}

	QDir d;
	d.setFilter (QDir::Dirs|QDir::NoDotAndDotDot);
	QFileInfoList list;
	foreach (QString path, searchpath) {
		d.setPath (path);
		list += d.entryInfoList();
	}

	foreach (QFileInfo fileInfo, list) {
		QPixmap p = Skin::getPixmap ("main", fileInfo.filePath());
		if (!p.isNull()) {
			new SkinChooserItem (QIcon (p), fileInfo.baseName (),
			                     fileInfo.absoluteFilePath (), this);
		}
	}

	connect (this, SIGNAL (itemClicked (QListWidgetItem *)),
	         this, SLOT (changeSkin (QListWidgetItem *)));
}

void
SkinList::changeSkin (QListWidgetItem *item)
{
	Skin *skin = Skin::getInstance ();
	SkinChooserItem *it = dynamic_cast<SkinChooserItem*> (item);

	QSettings settings;

	skin->setSkin (it->getPath ());
	settings.setValue ("skin/path", it->getPath ());
}


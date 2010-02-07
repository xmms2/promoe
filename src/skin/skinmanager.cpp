/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2010 XMMS2 Team
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

#include "skinmanager.h"
#include "skin.h"

#include <QSettings>
#include <QDebug>
#include <QStringList>
#include <QDir>

#include "promoe_config.h"

static const QString _defaultSkinPath = ":/skins/Almond-blue/";

static Skin * _defaultSkin = 0;

SkinManager::SkinManager() : QObject ()
{
	_defaultSkin = new Skin (_defaultSkinPath);

	QSettings settings;
	const QString skinPath = settings.value("skin/path", _defaultSkinPath)
	                                 .toString ();
	m_skin = _defaultSkin;
	if (skinPath != _defaultSkinPath) {
		Skin *skin = new Skin (_defaultSkin, skinPath);
		if (skin->isValid()) {
			m_skin = skin;
		} else {
			qDebug () << "Invalid Skin: " << skinPath;
			delete skin;
		}
	}
}

SkinManager * SkinManager::instance (void)
{
	static SkinManager *_instance =  new SkinManager ();
	return _instance;
}

Skin * SkinManager::activeSkin (void)
{
	return m_skin;
}

bool SkinManager::loadSkin (QString url)
{
	if (url.isEmpty ()) {
		return false;
	}

	if (url == m_skinPath) {
		// Already loaded
		return true;
	}

	Skin *skin;
	if (url == _defaultSkinPath) {
		skin = _defaultSkin;
	} else {
		skin = new Skin (_defaultSkin, url);
		if (!skin->isValid ()) {
			delete skin;
			return false;
		}
	}

	if (m_skin != _defaultSkin) {
		delete m_skin;
	}
	m_skin = skin;
	m_skinPath = url;

	QSettings settings;
	settings.setValue ("skin/path", url);

	emit skinChanged (skin);

	return true;
}

void SkinManager::emitSkinChanged (void)
{
	emit skinChanged (m_skin);
}

QStringList SkinManager::skinPathes () const
{
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
    // This should not be saved in the searchpath config value.
    searchpath.append (PROMOE_SKINDIR);

	return searchpath;
}

void SkinManager::setSkinPathes (QStringList pathes)
{
	QSettings settings;

	pathes.removeAll (PROMOE_SKINDIR);
	settings.setValue ("skin/searchpath", pathes);
}

#include "skinmanager.moc"

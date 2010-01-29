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

#ifndef __SKINMANAGER_H__
#define __SKINMANAGER_H__

#include <QObject>
#include <QStringList>

class Skin;

class SkinManager : public QObject
{
	Q_OBJECT
	public:
		static SkinManager *instance (void);

		Skin *activeSkin (void);
		bool loadSkin (QString url);

		QStringList skinPathes () const;
		void setSkinPathes (QStringList pathes);

		// Workaround for process startup
		void emitSkinChanged ();

	signals:
		void skinChanged (Skin *skin);

	protected:
		SkinManager (void);

		Skin *m_skin;
		QString m_skinPath;
};

#endif

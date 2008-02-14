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

#ifndef __ENTRYINFO_H__
#define __ENTRYINFO_H__

#include "ui_entryinfo.h"

#include <QDialog>

class XClientCache;

class EntryInfo : public QDialog, private Ui::EntryInfo {
	Q_OBJECT

	public:
		EntryInfo (QWidget *parent, XClientCache *cache, uint32_t id = 0);

	public slots:
		void setId (uint32_t id);

	private slots:
		void mlib_entry_changed (uint32_t id);
		void mlib_entry_removed (uint32_t id);

	private:
		void getInfo ();

		uint32_t m_id;
		XClientCache *m_cache;
};

#endif

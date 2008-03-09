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

#ifndef __URLOPEN_H__
#define __URLOPEN_H__

#include "ui_urlopen.h"

#include <QDialog>

class XCollection;

class UrlOpen : public QDialog,  private Ui::UrlOpen {
	Q_OBJECT

	public:
		UrlOpen (QWidget *parent, XCollection *coll);

	private slots:
		// autoconnect slots
		void on_openButton_clicked ();

	private:
		XCollection *m_collection;

};

#endif

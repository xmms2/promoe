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

#include <QUrl>

#include "urlopen.h"
#include "xcollection.h"

UrlOpen::UrlOpen (QWidget *parent, XCollection *coll) : QDialog (parent)
{
	setupUi (this);
	setAttribute (Qt::WA_DeleteOnClose);

	m_collection = coll;
}

void
UrlOpen::on_openButton_clicked ()
{
	QUrl url(urlEdit->text ());

	if (url.isValid ()) {
		bool b = m_collection->playlistAddUrl (url);
		if (b) {
			close ();
		}
	}
}

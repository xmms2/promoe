/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
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

#ifndef __FILEDIALOG_H__
#define __FILEDIALOG_H__

#include <QFileDialog>

class FileDialog : public QFileDialog
{
	Q_OBJECT
	public:
		FileDialog (QWidget *parent, const QString &name);
		QString getDirectory ();
		QStringList getFiles ();

	private:
		QString m_name;

};

#endif

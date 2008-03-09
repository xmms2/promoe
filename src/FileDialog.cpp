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

#include "FileDialog.h"

#include <QSettings>
#include <QFileDialog>
#include <QString>


FileDialog::FileDialog (QWidget *parent, const QString &name) : QFileDialog (parent)
{
	QSettings s;
	m_name = name;

	if (!s.contains("filedialog/" + name))
		s.setValue ("filedialog/" + name, QDir::homePath ());

	setDirectory (s.value("filedialog/" + name).toString ());
}

QStringList
FileDialog::getFiles ()
{
	QSettings s;
	QStringList ret = getOpenFileNames (NULL, "Choose files");
	if (ret.size() > 0)
		s.setValue ("filedialog/" + m_name, directory ().absolutePath ());

	qSort (ret);

	return ret;

}

QString
FileDialog::getDirectory ()
{
	QSettings s;
	QString ret = getExistingDirectory (NULL, "Choose directory");
	if (!ret.isNull())
		s.setValue ("filedialog/" + m_name, directory ().absolutePath ());

	return ret;

}

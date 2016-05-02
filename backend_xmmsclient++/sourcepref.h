/** 
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2008-2016 XMMS2 Team
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

/*
 * This file contains a hack to be able to honor source preferences when
 * converting a Propdicts to a QHash<QString, QVariant>.
 * It is only included in xclient.cpp and only used internally in that class
 *
 * The limitations that make this file necessary will be fixed with Coll2. So
 * this file can be removed as soon as Coll2 reaches xmms2-stabe
 *
 *
 * Minor note: At the time of writing this file, Coll2 wasn't in -devel, so
 * this might still take some time
 */

#ifndef __SOURCEPREF__
#define __SOURCEPREF__

#include <xmmsclient/xmmsclient++/dict.h>

#include <QList>
#include <QRegExp>

#include <QDebug>

/*
 * This class is used to get the source preference from a propdict as a
 * QList<QRegExp>.
 */
class MyPropDict : public Xmms::PropDict
{
	public:
		MyPropDict (const Xmms::PropDict &d) : PropDict (d) {}

		QList<QRegExp> getSourcePreference () {
			const char **sourcepref =
			           xmmsc_result_source_preference_get (result_);
			QList<QRegExp> prio_list;

			for (; *sourcepref; ++sourcepref) {
				prio_list.append (QRegExp (*sourcepref,
				                           Qt::CaseSensitive,
										   QRegExp::Wildcard));
			}

			return prio_list;
		}
};

int
getPriority (const QString source, const QList<QRegExp> prio_list) {
	for (int i=0; i < prio_list.size (); ++i) {
		if (prio_list[i].exactMatch (source))
			return i;
	}

	// If source doesn't match any expression in prio_list return an invalid
	// value. (The biggest valid value is prio_list.length () -1 )
	// In that case, the caller should not use the value corresponding with
	// this source
	return prio_list.size ();
}

#endif

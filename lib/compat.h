/** 
 *  This file is a part of Esperanza, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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


#ifndef __COMPAT_H__
#define __COMPAT_H__

#ifdef HAVE_XMMSV
#undef HAVE_XMMSV
#define HAVE_XMMSV 1
#else
#include <xmmsc/xmmsc_idnumbers.h>
#define HAVE_XMMSV (XMMS_IPC_PROTOCOL_VERSION >= 13)
#endif

#include <QString>
#include <QtDebug>

#if HAVE_XMMSV
#include <stdio.h>
#include <xmmsc/xmmsc_value.h>
#endif

inline QString
decodeXmmsUrl (const QString &path)
{
	QByteArray p_enc = path.toUtf8 ();
	QString ret;
#if HAVE_XMMSV
	// TODO: error checking...
	xmmsv_t *v_enc = xmmsv_new_string (p_enc.constData ());
	xmmsv_t *p_dec = xmmsv_decode_url (v_enc);
	xmmsv_unref (v_enc);
	const char *p;
	unsigned int p_len;
	xmmsv_get_bin (p_dec, reinterpret_cast<const unsigned char **>(&p), &p_len);
	ret = QString::fromUtf8 (p, p_len);
	xmmsv_unref (p_dec);
	// Free p?
#else
	char *p_dec = const_cast<char *> (xmmsc_result_decode_url (NULL, p_enc.constData ()));
	ret = QString::fromUtf8 (p_dec);
	free (p_dec);
#endif
	return ret;
}


#endif

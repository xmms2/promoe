/**
 *  This file is a part of Prome, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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

#ifndef __PLAYLISTSHADE_H__
#define __PLAYLISTSHADE_H__

#include <xmmsclient/xmmsclient++.h>
#include "Skin.h"

#include <QWidget>
#include <QHash>

class PlaylistShade : public QWidget {
	Q_OBJECT

	public:
		PlaylistShade (QWidget *parent);
		~PlaylistShade () {}

		void paintEvent (QPaintEvent *event);
		void mouseDoubleClickEvent (QMouseEvent *event);
		void setActive (bool);

	public slots:
		void setPixmaps (Skin *skin);
		void setMediainfo (const Xmms::PropDict &info);
		void settingsSaved ();

	private:
		QPixmap m_pixmap_le;
		QPixmap m_pixmap_re;
		QPixmap m_pixmap_mid;

		QPixmap m_pixmap_re_0;
		QPixmap m_pixmap_re_1;

		QFont m_font;
		QColor m_color;
		QString m_text;
//		QFontMetrics m_fm;
};

#endif

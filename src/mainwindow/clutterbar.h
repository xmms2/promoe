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

#ifndef __CLUTTERBAR_H__
#define __CLUTTERBAR_H__

#include "QWidget"
class QMouseEvent;
class QPaintEvent;

class Skin;

class ClutterBar : public QWidget
{
	Q_OBJECT
	public:
		ClutterBar (QWidget *parent);
		~ClutterBar ();

	public slots:
		void setPixmaps(Skin *skin);

	protected:
		void paintEvent (QPaintEvent *event);

		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);

		QPixmap m_clutter_off;
		QPixmap m_clutter_on;

		QPixmap m_clutter_o;
		QPixmap m_clutter_a;
		QPixmap m_clutter_i;
		QPixmap m_clutter_d;
		QPixmap m_clutter_v;

		bool enabled;

		QPixmap m_pixmap;
};


#endif

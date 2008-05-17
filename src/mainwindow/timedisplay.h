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

#ifndef __TIMEDISPLAY_H__
#define __TIMEDISPLAY_H__

#include <QWidget>
#include <QMap>

class QMouseEvent;
class QPaintEvent;
class QPixmap;

typedef QMap<int, QPixmap> PixmapMap;

class TimeDisplay : public QWidget
{
	Q_OBJECT
	public:
		TimeDisplay (QWidget *parent);
		~TimeDisplay () {};
		void setTime (int);

	public slots:
		void setPixmaps (const PixmapMap &p) {m_pixmaps = p;}

	signals:
		void clicked(void);

	protected:
		void mousePressEvent (QMouseEvent *event) {};
		void mouseReleaseEvent (QMouseEvent *event);
		void paintEvent (QPaintEvent *event);

		int m_time;
		PixmapMap m_pixmaps;
};

#endif

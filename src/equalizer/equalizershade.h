/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2010 XMMS2 Team
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

#ifndef __EQUALIZERSHADE_H__
#define __EQUALIZERSHADE_H__

#include <QWidget>
#include <QPixmap>

class QPaintEvent;
class QMouseEvent;

class EqualizerWindow;
class Skin;

class EqualizerShade : public QWidget
{
	Q_OBJECT

	public:
		EqualizerShade (EqualizerWindow *parent);

		void paintEvent (QPaintEvent *event);
		void mouseDoubleClickEvent (QMouseEvent *event);
		void setActive (bool);

	public slots:
		void setPixmaps (Skin *);

	private:
		bool m_active;
		QPixmap p_active;
		QPixmap p_inactive;
};

#endif

/**
 *  This file is a part of Promoe, an XMMS2 CLient
 *
 *  Copyright (C) 2008-2009 XMMS2 Team
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

#ifndef __PIXMAPBUTTON_H__
#define __PIXMAPBUTTON_H__

#include <QAbstractButton>

class QPaintEvent;
class QWidget;

/*
 * QIcon::Normal for normal apperance
 * QIcon::Active for pressed state
 *
 * QIcon::Off for uncecked state
 * QIcon::On for checked state
 *
 * only QIcon::Normal QIcon::Off combination is necessary
 */

class PixmapButton : public QAbstractButton {
	Q_OBJECT

	public:
		PixmapButton (QWidget *parent) : QAbstractButton (parent) {}
		~PixmapButton () {};

	protected:
		void paintEvent ( QPaintEvent * event );

};

#endif

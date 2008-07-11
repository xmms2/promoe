/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
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

#ifndef __PIXMAPNUMBERDISPLAY_H__
#define __PIXMAPNUMBERDISPLAY_H__

#include <QWidget>
#include <QMap>

class QPaintEvent;
class Skin;

typedef QMap<int, QPixmap> PixmapMap;

class PixmapNumberDisplay : public QWidget
{
	Q_OBJECT;

	public:
		PixmapNumberDisplay (QWidget *parent);
		~PixmapNumberDisplay () { };

		void setValue (int value);
		void setDigits (unsigned int digits);
		void setPixmaps (PixmapMap numbers);

	private:
		int m_value;
		unsigned int m_digits;
		PixmapMap m_numbers;
		
		int getDigitAtPos (unsigned int position);
		void paintEvent (QPaintEvent *event);
};

#endif

/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
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

#include "pixmapnumberdisplay.h"
#include "math.h"

#include <QPainter>

PixmapNumberDisplay::PixmapNumberDisplay (QWidget *parent) : QWidget (parent)
{
	m_value = 0;
	m_digits = 2;
}

void
PixmapNumberDisplay::setPixmaps (PixmapMap numbers)
{
	m_numbers = numbers;
	update ();
}

void
PixmapNumberDisplay::setValue (int value)
{
	if (m_value == value)
		return;

	m_value = value;
	update ();
}

void
PixmapNumberDisplay::setDigits (unsigned int digits)
{
	if (m_digits == digits)
		return;

	m_digits = digits;
	update ();
}

int
PixmapNumberDisplay::getDigitAtPos (unsigned int position) {
	if (m_value == 0) {
		if (position == 1)
			return 0;
		else
			return -1;
	}
	if ( m_value / (unsigned int) pow (10, position - 1) == 0)
		return -1;

	return (unsigned int) (m_value / pow (10, position - 1)) % 10;
}

void
PixmapNumberDisplay::paintEvent (QPaintEvent *event)
{
	QPainter paint;
	paint.begin (this);

	for (unsigned int i = 0; i < m_digits; i++) {
		paint.drawPixmap (i*5, 0,
		                  m_numbers[getDigitAtPos (m_digits - i)]);
	}

	paint.end ();
}

#include "pixmapnumberdisplay.moc"

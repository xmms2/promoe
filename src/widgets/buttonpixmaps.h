/**
 *  This file is a part of Promoe, an XMMS2 CLient
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

#ifndef __BUTTONPIXMAPS_H__
#define __BUTTONPIXMAPS_H__

#include <QMap>
#include <QPixmap>

typedef QMap <int, QPixmap> PixmapMap;

/*
 * QIcon::Normal for normal apperance
 * QIcon::Active for pressed state
 *
 * QIcon::Off for uncecked state
 * QIcon::On for checked state
 *
 * only QIcon::Normal QIcon::Off combination is necessary
 */
class ButtonPixmaps
{
	public:
		ButtonPixmaps () :
			m_pixmaps() {}
		ButtonPixmaps (const ButtonPixmaps &p) :
			m_pixmaps(p.m_pixmaps) {}


		enum Mode {
			Normal    = 0, // Used when button is in active Window
			Inactive  = 1, // Used when button is in inactive Window
			Pressed   = 2  // used when button is pressed
		};

		enum State {
			Unchecked = 0, // used when button is unchecked
			Checked   = 1  // used when button is checked
		};

		void addPixmap (const QPixmap & pixmap, Mode m = Normal,
		                State s = Unchecked) {
			m_pixmaps[getKey(m, s)] = pixmap;
			if (!m_pixmaps.contains (getKey (Normal, Unchecked))) {
				m_pixmaps[getKey (Normal, Unchecked)] = pixmap;
			}
		}

		QPixmap pixmap (Mode m = Normal, State s = Unchecked) const {
			if (m_pixmaps.contains (getKey (m, s))) {
				return m_pixmaps.value (getKey (m, s));

			} else if (s == Checked && m != Normal &&
			           m_pixmaps.contains (getKey (Normal, s))) {
				return m_pixmaps.value (getKey (Normal, s));
			} else {
				return m_pixmaps.value (getKey (Normal, Unchecked));
			}
		}


		ButtonPixmaps & operator= (const ButtonPixmaps & p) {
			m_pixmaps = p.m_pixmaps;
		}

	protected:
		PixmapMap m_pixmaps;

		// get the key to be used in the PixmapMap
		inline int getKey (Mode m, State s) const { return (m + s*3); }
};

typedef ButtonPixmaps PBPixmaps;
typedef ButtonPixmaps PixmapButtonPixmaps;

#endif

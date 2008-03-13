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

#ifndef __VOLUMESLIDER_H__
#define __VOLUMESLIDER_H__

#include "pixmapslider.h"
class Skin;

class Slider : public PixmapSlider
{
	Q_OBJECT
	public:
		Slider (QWidget *parent, uint min, uint max, uint on, uint off, int, int);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		uint m_name_min;
		uint m_name_max;
		uint m_name_on;
		uint m_name_off;
};

#endif

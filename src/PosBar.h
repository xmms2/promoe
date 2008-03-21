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

#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "pixmapslider.h"

class Skin;

class PosBar : public PixmapSlider
{
	Q_OBJECT

	public:
		PosBar (QWidget *parent, uint bg, uint bnormal, uint bpressed);
		~PosBar () { }
		
	public slots:
		void setPixmaps (Skin *skin);
		void seekMs (int);

	private:
		uint m_slider_normal;
		uint m_slider_pressed;
		uint m_bg;
};

#endif

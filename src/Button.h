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

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "pixmapbutton.h"

class Skin;

class Button : public PixmapButton
{
	Q_OBJECT
	public:
		Button (QWidget *parent, uint btn1, uint btn2, bool pls=false);
		Button (QWidget *parent);
		~Button ();

	public slots:
		void setPixmaps (Skin *skin);

	protected:
		uint m_name_normal;
		uint m_name_pressed;

		bool m_pls;
};

class ToggleButton : public PixmapButton
{
	Q_OBJECT
	public:
		ToggleButton (QWidget *parent, uint, uint, uint, uint);
		~ToggleButton () {}

	public slots:
		void setPixmaps(Skin *skin);

	private:
		uint m_name_on_normal;
		uint m_name_on_pressed;
		uint m_name_off_normal;
		uint m_name_off_pressed;
};

#endif

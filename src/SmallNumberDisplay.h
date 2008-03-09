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

#ifndef __SMALLNUMBERDISPLAY_H__
#define __SMALLNUMBERDISPLAY_H__

#include "PixWidget.h"
#include <QPainter>


class SmallNumberDisplay : public PixWidget
{
	Q_OBJECT;

	public:
		SmallNumberDisplay (QWidget *parent, int w);
		~SmallNumberDisplay () { };

		void setNumber (int num, int len);
		int getNumber (void) const { return m_num; }

	public slots:
		void setPixmaps (Skin *skin);

	private:
		char m_nums[2];
		int m_num;
		int m_w;

		void drawNumber (void);
};

#endif

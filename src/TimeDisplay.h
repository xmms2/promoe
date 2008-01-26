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

#ifndef __TIMEDISPLAY_H__
#define __TIMEDISPLAY_H__

class TimeDisplay;

#include "PixWidget.h"
#include "NumberDisplay.h"

class TimeDisplay : public PixWidget
{
	Q_OBJECT
	public:
		TimeDisplay (QWidget *parent, int time);
		~TimeDisplay ();
		void setTime (int);
		void drawMinus();
	public slots:
		void setPixmaps (Skin *skin);
	signals:
		void clicked(void);

	protected:
		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);
		int m_time;
		NumberDisplay *m_number_min;
		NumberDisplay *m_number_sec;

		uint m_w;
};

#endif

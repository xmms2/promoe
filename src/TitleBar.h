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

#ifndef __TITLEBAR_H__
#define __TITLEBAR_H__

class TitleBar;

#include "PixWidget.h"

class Button;
class Skin;

class TitleBar : public PixWidget
{
	Q_OBJECT
	public:
		TitleBar (QWidget *parent, bool shaded);
		~TitleBar ();
		void setActive (bool active);

	public slots:
		void setPixmaps (Skin *skin);
		void showMenu (void);
		void showServerB (void);
		void showTheme (void);
		void showMlib (void);
		void showSettings (void);

	protected:
		void mouseDoubleClickEvent (QMouseEvent *event);

	private:
		QPixmap m_pixmap_active;
		QPixmap m_pixmap_inactive;

		Button *m_shadebtn;
		Button *m_menubtn;
		Button *m_closebtn;
		Button *m_minimize;

		bool m_shaded;
};

#endif

/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2016 XMMS2 Team
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

#include "QWidget"

class QPaintEvent;
class PixmapButton;
class Skin;

class TitleBar : public QWidget
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

	protected slots:
		void paintEvent (QPaintEvent *event);

	protected:
		void mouseDoubleClickEvent (QMouseEvent *event);

	private:
		QPixmap m_pixmap_active;
		QPixmap m_pixmap_inactive;

		PixmapButton *m_shadebtn;
		PixmapButton *m_menubtn;
		PixmapButton *m_closebtn;
		PixmapButton *m_minimize;

		bool m_shaded;
		QPixmap m_pixmap;
};

#endif

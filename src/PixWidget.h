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

#ifndef __PIXWIDGET_H__
#define __PIXWIDGET_H__

#include <QWidget>
class QPixmap;

class Skin;

class PixWidget : public QWidget
{
	Q_OBJECT
	public:
		PixWidget(QWidget *parent = 0);
		~PixWidget();
		void paintEvent (QPaintEvent *event);
	public slots:
		virtual void setPixmaps(Skin *skin);
	protected:
		QPixmap m_pixmap;
};

#endif

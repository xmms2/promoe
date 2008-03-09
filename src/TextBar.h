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

#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

#include <iostream>

#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QFont>
#include <QTimer>
#include <QBrush>
#include <QPalette>
#include <QBitmap>

#include "Skin.h"

class TextScroller : public QWidget
{
	Q_OBJECT

	public:
		TextScroller (QWidget *parent, uint, uint, const QString &);
		~TextScroller ();

		void setText(QString text);
		void setFontSize (int i) { m_fontsize = i; }
		void setTTF (bool b) { m_ttf = b; }

	public slots:
		void addOffset ();
		void setPixmaps(Skin *skin);
		void settingsSaved (void);

	protected:
		QPixmap m_pixmap;
		void paintEvent (QPaintEvent *event);

	private:
		int m_w;
		int m_h;
		int m_x_off;
		int m_x2_off;
		int m_fontsize;
		bool m_ttf;
		bool m_scroll;

		QTimer *m_timer;
		QString m_text;
		QString m_name;

		void drawBitmapFont (QString text);
		void drawQtFont (QString text);
};

#endif

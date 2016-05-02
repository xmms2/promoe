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

#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

#include <QWidget>
class QMouseEvent;
class QPixmap;
class QTimer;

class Skin;

class TextScroller : public QWidget
{
	Q_OBJECT

	public:
		TextScroller (QWidget *parent, uint, uint, const QString &);
		~TextScroller () {}

		void setText(QString text);
		void setFontSize (int i) { m_fontsize = i; }
		void setTTF (bool b) { m_ttf = b; }

	public slots:
		void addOffset ();
		void setPixmaps(Skin *skin);
		void settingsChanged (void);

	protected:
		void paintEvent (QPaintEvent *event);
		void mousePressEvent (QMouseEvent * event);
		void mouseReleaseEvent (QMouseEvent * event);
		void mouseMoveEvent (QMouseEvent * event);

	private:
		int m_x_off;
		int m_fontsize;
		int m_drag_off;
		bool m_ttf;
		bool m_scroll;
		bool m_dragtext;

		QTimer *m_timer;
		QString m_text;
		QString m_name;
		QPixmap m_pixmap;

		void drawText ();
		void drawBitmapFont (QString text);
		void drawQtFont (QString text);
		void updateScrolling ();
};

#endif

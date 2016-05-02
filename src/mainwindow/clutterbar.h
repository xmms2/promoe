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

#ifndef __CLUTTERBAR_H__
#define __CLUTTERBAR_H__

#include <QWidget>
#include <QMap>
#include <QPixmap>

class QMouseEvent;
class QPaintEvent;

class Skin;

typedef QList<QPixmap> QPixmapList;

class ClutterBar : public QWidget
{
	Q_OBJECT
	public:
		ClutterBar (QWidget *parent);
		~ClutterBar ();

	public slots:
		void setPixmaps (const QPixmapList &);
		void setPixmaps(Skin *skin);

	signals:
		void clicked_o ();
		void clicked_a (bool checked);
		void clicked_i ();
		void clicked_d (bool checked);
		void clicked_v ();

	protected:
		void paintEvent (QPaintEvent *event);

		void mousePressEvent (QMouseEvent *event);
		void mouseMoveEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);

		void enterEvent (QEvent *event);
		void leaveEvent (QEvent *event);

		bool posInWidget (const int x, const int y) const;
		int posToVal (const int x, const int y) const;

		QPixmapList m_pixmaps;

		int m_val;
		bool m_checked_a;
		bool m_checked_d;

		bool m_show_always;
};


#endif

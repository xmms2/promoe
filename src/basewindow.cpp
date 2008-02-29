/**
 *  This file is a part of promoe, an XMMS2 Client
 *
 *  Copyright (C) 2008 Thomas Frauendorfer
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "basewindow.h"

#include <QApplication>
#include <QWidgetList>
#include <QMouseEvent>
#include <QPoint>

BaseWindow::BaseWindow (QWidget *parent) : QMainWindow (parent)
{
}

void
BaseWindow::mousePressEvent (QMouseEvent *event)
{
	if (event->buttons () & Qt::LeftButton) {
		m_diff = event->pos ();
	}
}

void
BaseWindow::mouseReleaseEvent (QMouseEvent *event)
{
	if (event->buttons () & Qt::LeftButton) {
		m_diff = QPoint (0, 0);
	}
}

void
BaseWindow::mouseMoveEvent (QMouseEvent *event)
{
	// Only move, if the left button is pressed
	if ((event->buttons () & Qt::LeftButton) && !m_diff.isNull ()) {
		move (snapWindow (event->globalPos() - m_diff));
	}

}

QPoint
BaseWindow::snapWindow(QPoint pos)
{
	//TODO: make snapdistance configurable
	qint32 snapdistance = 10;

	qint32 left = pos.x ();
	qint32 right = left + width ();
	qint32 top = pos.y ();
	qint32 bottom = top + height ();

	qDebug ("top: %i, bottom: %i, left: %i, right: %i", top, bottom, left, right);
	QWidgetList widgets = qApp->topLevelWidgets ();
	QWidget *w;
	// snap to left or right edge
	bool snappedV = false;
	bool snappedH = false;
	foreach (w, widgets) {
		if (w == this) {
			continue;
		}
		if (!w->isVisible ()) {
			continue;
		}
		qint32 w_left = w->x ();
		qint32 w_right = w_left + w->width ();
		qint32 w_top = w->y ();
		qint32 w_bottom = w_top + w->height ();
		// test if we are anywhere near this widget
		if ((w_top > bottom + snapdistance) ||
		    (w_bottom < top - snapdistance) ||
			(w_left > right + snapdistance) ||
			(w_right < left - snapdistance)) {
			continue;
		}

		// test if this widget can snap left or right to another widget
		// and if it can, test if the tops or bottoms can also snap
		if (!snappedV && (qAbs (w_left - right) < snapdistance)) {
			pos.setX (w_left - width ());
			snappedV = true;
			if (snappedH) {
				break;
			}
			if (qAbs (w_bottom - bottom) < snapdistance) {
				pos.setY (w_bottom - height ());
				break;
			} else if (qAbs (w_top - top) < snapdistance) {
				pos.setY (w_top);
				break;
			}
		} else if (!snappedV && (qAbs (left - w_right) < snapdistance)) {
			pos.setX (w_right);
			snappedV = true;
			if (snappedH) {
				break;
			}
			if (qAbs (w_bottom - bottom) < snapdistance) {
				pos.setY (w_bottom - height ());
				break;
			} else if (qAbs (w_top - top) < snapdistance) {
				pos.setY (w_top);
				break;
			}
		}
		// test if this widget can snap to top or bottom of another widget
		// and if it can, test if the left or right edges also can
		if (!snappedH && (qAbs (top - w_bottom) < snapdistance)) {
			pos.setY (w_bottom);
			snappedH = true;
			if (snappedV) {
				break;
			}
			if (qAbs (w_left - left) < snapdistance) {
				pos.setX (w_left);
				break;
			} else if (qAbs (w_right - right) < snapdistance) {
				pos.setX (w_right - width ());
				break;
			}
		} else if (!snappedH && (qAbs (w_top - bottom) < snapdistance)) {
			pos.setY (w_top - height ());
			snappedH = true;
			if (snappedV) {
				break;
			}
			if (qAbs (w_left - left) < snapdistance) {
				pos.setX (w_left);
				break;
			} else if (qAbs (w_right - right) < snapdistance) {
				pos.setX (w_right - width ());
				break;
			}

		}
	}

	return pos;
}

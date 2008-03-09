/**
 *  This file is a part of Promoe, an XMMS2 Client
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
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
#include "mainwindow.h"

#include <QApplication>
#include <QWidgetList>
#include <QMouseEvent>
#include <QPoint>

BaseWindow::BaseWindow (QWidget *parent) : QMainWindow (parent)
{
}

bool
BaseWindow::touches (QWidget *widget)
{
	if (this == widget) {
		return true;
	}

	qint32 left = x ();
	qint32 right = left + width ();
	qint32 top = y ();
	qint32 bottom = top + height ();

	qint32 w_left = widget->x ();
	qint32 w_right = w_left + widget->width ();
	qint32 w_top = widget->y ();
	qint32 w_bottom = w_top + widget->height ();

	if (( (top <= w_bottom) && (bottom >= w_top) &&
	      ((left == w_right || right == w_left))   ) ||
		( (left <= w_right) && (right >= w_left) &&
		  ((top == w_bottom) || (bottom == w_top) )  )) {
		return true;
	}

	return false;
}

MainWindow *
BaseWindow::mw ()
{
	//MainWindow is the only BaseWindow without a *parent
	if (parent ()) {
		return qobject_cast<MainWindow *>(parent ());
	} else {
		return qobject_cast<MainWindow *>(this);
	}
}

void
BaseWindow::mousePressEvent (QMouseEvent *event)
{
	if (event->button () == Qt::LeftButton) {
		m_diff = event->pos ();
	}
}

void
BaseWindow::mouseReleaseEvent (QMouseEvent *event)
{
	if (event->button () == Qt::LeftButton) {
		m_diff = QPoint (0, 0);
		mw ()->attachWidgets ();
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
BaseWindow::snapWindow(QPoint pos, AttachedWindowMap attached)
{
	//TODO: make snapdistance configurable
	qint32 snapdistance = 10;

	QWidgetList widgets = qApp->topLevelWidgets ();
	BaseWindow *b;
	widgets.removeAt (widgets.indexOf (this));
	// ignore attached widgets
	// attached Widgets touch this window, and would create a strange movement
	if (!attached.isEmpty ()) {
		foreach (b, attached.keys ()) {
			int i = widgets.indexOf (b);
			if (i >= 0) {
				widgets.removeAt (i);
			}
		}
	}

	bool vSnapped = false;
	bool hSnapped = false;

	attached.insert (this, QPoint (0,0));
	BaseWindow *ref;

	// equalizerwindow and playlistwindow have only process this method once
	// the ugly part comes into play, because mainwindow also snaps to
	// windows it's subwindows get near.
	// We iterate over the moved window and all it's attached windows.
	// if a attached window can snap to a window, we use the offset of
	// this windows position to the mainwindow to calculate the position
	// we have to move the mainwindow to.
	//
	// As soon as we have a horizontal an vertical postition where the
	// windows can snap to, we stop further processing
	// (vSnapped and hSnapped are used to keep track of that)
	foreach (ref, attached.keys ()) {
		qint32 left = pos.x () + attached[ref].x ();
		qint32 right = left + ref->width ();
		qint32 top = pos.y () + attached[ref].y ();
		qint32 bottom = top + ref->height ();

		QWidget *w;

		// snap to left or right edge
		foreach (w, widgets) {
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
			if ((qAbs (w_left - right) < snapdistance)) {
				if (!vSnapped) {
					pos.setX (w_left - ref->width () - attached[ref].x ());
					vSnapped = true;
				}
				if (hSnapped) {
					break;
				}
				if (qAbs (w_bottom - bottom) < snapdistance) {
					pos.setY (w_bottom - ref->height () - attached[ref].y ());
					hSnapped = true;
					break;
				} else if (qAbs (w_top - top) < snapdistance) {
					pos.setY (w_top - attached[ref].y ());
					hSnapped = true;
					break;
				}
			} else if ((qAbs (left - w_right) < snapdistance)) {
				if (!vSnapped) {
					pos.setX (w_right - attached[ref].x ());
					vSnapped = true;
				}
				if (hSnapped) {
					break;
				}
				if (qAbs (w_bottom - bottom) < snapdistance) {
					pos.setY (w_bottom - ref->height () - attached[ref].y ());
					hSnapped = true;
					break;
				} else if (qAbs (w_top - top) < snapdistance) {
					pos.setY (w_top - attached[ref].y ());
					hSnapped = true;
					break;
				}
			}
			// test if this widget can snap to top or bottom of another widget
			// and if it can, test if the left or right edges also can
			if ((qAbs (top - w_bottom) < snapdistance)) {
				if (!hSnapped) {
					pos.setY (w_bottom - attached[ref].y ());
					hSnapped = true;
				}
				if (vSnapped) {
					break;
				}
				if (qAbs (w_left - left) < snapdistance) {
					pos.setX (w_left - attached[ref].x ());
					vSnapped = true;
					break;
				} else if (qAbs (w_right - right) < snapdistance) {
					pos.setX (w_right - ref->width () - attached[ref].x ());
					vSnapped = true;
					break;
				}
			} else if ((qAbs (w_top - bottom) < snapdistance)) {
				if (!hSnapped) {
					pos.setY (w_top - ref->height () - attached[ref].y ());
					hSnapped = true;
				}
				if (vSnapped) {
					break;
				}
				if (qAbs (w_left - left) < snapdistance) {
					pos.setX (w_left - attached[ref].x ());
					vSnapped = true;
					break;
				} else if (qAbs (w_right - right) < snapdistance) {
					pos.setX (w_right - ref->width () - attached[ref].x ());
					vSnapped = true;
					break;
				}

			}
		} // end foreach (w, widgets)

		if (hSnapped && vSnapped) {
			break;
		}
	} // end foreach (ref, attached)

	return pos;
}

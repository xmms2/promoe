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

#ifndef __EQUALIZERWINDOW_H__
#define __EQUALIZERWINDOW_H__

#include "basewindow.h"

class QWidget;
class QMoveEvent;
class QHideEvent;
class QShowEvent;

class MainWindow;
class EqualizerWidget;

class EqualizerWindow : public BaseWindow
{
	Q_OBJECT
	public:
		EqualizerWindow(QWidget *parent);
		~EqualizerWindow() {}

	signals:
		void visibilityChanged(bool visible);

	public slots:
		void setEnabled (void);

	protected:
		void hideEvent (QHideEvent *event);
		void showEvent (QShowEvent *event);
		void moveEvent(QMoveEvent *event);


	private:
		MainWindow *m_mw;
		EqualizerWidget *m_equalizer;
};

#endif // __EQUALIZERWINDOW_H__

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

#ifndef __STEREOMONO_H__
#define __STEREOMONO_H__

#include <QWidget>

class QPaintEvent;
class Skin;

class StereoMono : public QWidget
{
	Q_OBJECT
	public:
		StereoMono (QWidget *parent);
		~StereoMono () { }

		void setStereoMono (bool, bool);

	public slots:
		void setPixmaps (Skin *skin);

	protected slots:
		void paintEvent (QPaintEvent *event);

	private:
		bool m_stereo;
		bool m_mono;

		QPixmap m_pixmap_stereo_on;
		QPixmap m_pixmap_stereo_off;
		QPixmap m_pixmap_mono_on;
		QPixmap m_pixmap_mono_off;
};

#endif

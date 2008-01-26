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

#ifndef __PLAYSTATUS_H__
#define __PLAYSTATUS_H__

#include <xmmsclient/xmmsclient++.h>
#include "XMMSHandler.h"
#include "PixWidget.h"

class PlayStatus : public PixWidget
{
	Q_OBJECT
	public:
		PlayStatus (QWidget *parent);
		~PlayStatus () { }


	public slots:
		void setPixmaps (Skin *skin);
		void setStatus (Xmms::Playback::Status status);

	private:
		Xmms::Playback::Status m_status;

		QPixmap m_pixmap_stop;
		QPixmap m_pixmap_play;
		QPixmap m_pixmap_pause;
};

#endif

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

#ifndef __SHADEDDISPLAY_H__
#define __SHADEDDISPLAY_H__

class ShadedDisplay;

#include <xmmsclient/xmmsclient++.h>
#include "Display.h"
#include <QHash>
#include <QVariant>

typedef QHash<QString, QVariant> QVariantHash;

class MainWindow;
class Skin;
class PixmapButton;
class SmallTimeDisplay;
class SmallNumberDisplay;
class TextScroller;

class ShadedDisplay : public SkinDisplay
{
	Q_OBJECT
	public:
		ShadedDisplay (QWidget *parent);
		~ShadedDisplay () { }

		SmallNumberDisplay *m_number;
		SmallNumberDisplay *m_number2;
		TextScroller *m_title;

	private:
		int32_t m_duration;
		SmallTimeDisplay *m_time;

		PixmapButton *m_prev;
		PixmapButton *m_play;
		PixmapButton *m_pause;
		PixmapButton *m_stop;
		PixmapButton *m_next;
		PixmapButton *m_eject;

	public slots:
		void setStatus (Xmms::Playback::Status status);
		void setPlaytime (uint32_t time);
		void setMediainfo (QVariantHash info);
		void setPixmaps (Skin *skin);
};

#endif

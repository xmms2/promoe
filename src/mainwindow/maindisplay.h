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

#ifndef __MAINDISPLAY_H__
#define __MAINDISPLAY_H__

class MainDisplay;

#include <xmmsclient/xmmsclient++.h>
#include "XMMSHandler.h"

#include <iostream>

#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QHash>
#include <QBrush>
#include <QPalette>

#include "Display.h"

class PixmapButton;
class TextScroller;
class TimeDisplay;
class SmallNumberDisplay;
class StereoMono;
class PosBar;
class Slider;
class PlayStatus;
class MainWindow;
class ClutterBar;
class XConfig;

class MainDisplay : public SkinDisplay
{
	Q_OBJECT
	public:
		MainDisplay (QWidget *parent);
		~MainDisplay () {}

		TextScroller *m_text;
		TimeDisplay *m_time;

		SmallNumberDisplay *m_kbps;
		SmallNumberDisplay *m_khz;

		StereoMono *m_stereo;
		PosBar *m_posbar;
		Slider *m_vslider;
		Slider *m_bslider;

		PlayStatus *m_playstatus;
		MainWindow *getMW(void) { return m_mw; }

	public slots:
		void setPixmaps(Skin *skin);
		void setStatus (Xmms::Playback::Status status);
		void setPlaytime (uint32_t time);
		void setMediainfo (const Xmms::PropDict &);
		void toggleTime(void);
		void updateVolume (uint volume);
		void setVolume (int volume);

	protected slots:
		void serverConfigChanged (QString key, QString value);
		void setRepeatAllEnabled (bool enabled);

	protected:
		void SetupPushButtons (void);
		void SetupToggleButtons (void);

		PixmapButton *m_prev;
		PixmapButton *m_play;
		PixmapButton *m_pause;
		PixmapButton *m_stop;
		PixmapButton *m_next;
		PixmapButton *m_eject;

		PixmapButton *m_pls;
		PixmapButton *m_eq;
		PixmapButton *m_shuffle;
		PixmapButton *m_repeat;

		MainWindow *m_mw;

		ClutterBar *m_clutterbar;

	private:
		void setupServerConfig ();

		XConfig *m_xconfig;

};

#endif

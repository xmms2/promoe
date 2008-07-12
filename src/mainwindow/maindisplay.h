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
#include <QHash>
#include <QVariant>

#include "Display.h"

typedef QHash<QString, QVariant> QVariantHash;

class PixmapButton;
class PixmapNumberDisplay;
class PixmapSlider;
class TextScroller;
class TimeDisplay;
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

		StereoMono *m_stereo;
		PosBar *m_posbar;
		PixmapSlider *m_vslider;
		PixmapSlider *m_bslider;

		PlayStatus *m_playstatus;
		MainWindow *getMW(void) { return m_mw; }

	public slots:
		void setPixmaps(Skin *skin);
		void setStatus (Xmms::Playback::Status status);
		void setPlaytime (uint32_t time);
//		void setMediainfo (const Xmms::PropDict &);
		void setMediainfo (const QVariantHash);
		void updateVolume (uint volume);
		void setVolume (int volume);

	protected slots:
		void serverConfigChanged (QString key, QString value);
		void setRepeatAllEnabled (bool enabled);
		void handleDisconnected ();
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

		PixmapNumberDisplay *m_kbps;
		PixmapNumberDisplay *m_khz;

		MainWindow *m_mw;

		ClutterBar *m_clutterbar;

	private:
		void setupServerConfig ();

		XConfig *m_xconfig;

};

#endif

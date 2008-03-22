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

#include <xmmsclient/xmmsclient++.h>
#include "XMMSHandler.h"
#include "xclientcache.h"
#include "xplayback.h"
#include "xconfig.h"

#include "maindisplay.h"
#include "mainwindow.h"

#include "TitleBar.h"
#include "Button.h"
#include "TextBar.h"
#include "NumberDisplay.h"
#include "TimeDisplay.h"
#include "SmallNumberDisplay.h"
#include "stereomono.h"
#include "PosBar.h"
#include "PlayStatus.h"
#include "VolumeSlider.h"
#include "playlistwindow.h"
#include "equalizerwindow.h"
#include "clutterbar.h"

#include <QFileDialog>
#include <QSettings>

MainDisplay::MainDisplay (QWidget *parent) : SkinDisplay(parent)
{
	XMMSHandler &client = XMMSHandler::getInstance ();
	m_xconfig = client.xconfig ();

	m_tbar = new TitleBar(this, false);
	m_tbar->move(0, 0);
	m_tbar->resize(275, 14);

	m_mw = dynamic_cast<MainWindow *>(parent);

	SetupPushButtons ();
	SetupToggleButtons ();

	m_text = new TextScroller (this, 154, 10, "main");
	m_text->move (112, 25);

	m_time = new TimeDisplay(this, 0);
	connect (m_time, SIGNAL(clicked()), this, SLOT(toggleTime()));

	m_kbps = new SmallNumberDisplay (this, 15);
	m_kbps->move (111, 43);
	m_kbps->setNumber (128, 3);

	m_khz = new SmallNumberDisplay (this, 10);
	m_khz->move (156, 43);
	m_khz->setNumber (44, 2);

	m_stereo = new StereoMono (this);
	m_stereo->move (212, 41);
	m_stereo->setStereoMono (0, 0);

	m_clutterbar = new ClutterBar (this);
	m_clutterbar->move (10, 22);

	m_posbar = new PosBar (this, Skin::POSBAR, 
						   Skin::POSBAR_BTN_0, 
						   Skin::POSBAR_BTN_1);
	m_posbar->move (16, 72);

	m_playstatus = new PlayStatus (this);
	m_playstatus->move (24, 28);

	m_vslider = new Slider(this, Skin::VOLUMEBAR_POS_0, Skin::VOLUMEBAR_POS_27,
	                       Skin::VOLBAR_BTN_0, Skin::VOLBAR_BTN_1, 0, 100);
	m_vslider->move (107, 57);

	m_bslider = new Slider(this, Skin::BALANCE_POS_0, Skin::BALANCE_POS_27,
	                       Skin::BALANCE_BTN_0, Skin::BALANCE_BTN_1, -20, 20);
	m_bslider->move (177, 57);

	connect (&client, SIGNAL(currentSong (const Xmms::PropDict &)), 
			 this, SLOT(setMediainfo (const Xmms::PropDict &)));
	connect (&client, SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(setStatus(Xmms::Playback::Status)));
//	connect (&xmmsh, SIGNAL(playtimeChanged(uint)),
//	         this, SLOT(setPlaytime(uint)));
	connect (client.cache () , SIGNAL (playtime (uint32_t)),
	         this,  SLOT (setPlaytime (uint32_t)));
	connect (&client, SIGNAL(getVolume(uint)), this, SLOT(updateVolume(uint)));
	connect (m_vslider, SIGNAL(sliderMoved(int)), this, SLOT(setVolume(int)));
	client.volumeGet();

	setupServerConfig ();
}


void
MainDisplay::updateVolume (uint volume)
{
	m_vslider->setValue((int)volume);
}

void
MainDisplay::setVolume (int volume)
{
	XMMSHandler &xmmsh = XMMSHandler::getInstance();
	xmmsh.volumeSet((uint)volume);
}


void
MainDisplay::setPixmaps (Skin *skin)
{
	QPalette palette = QPalette();
	QBrush brush = QBrush(skin->getItem(Skin::MAIN_WINDOW));
	palette.setBrush(QPalette::Background, brush);
	setPalette(palette);

	setFixedSize(QSize(275, 116));
}

void
MainDisplay::setStatus (Xmms::Playback::Status status)
{
	m_playstatus->setStatus (status);
	
	if (status == Xmms::Playback::STOPPED) {
		m_time->setTime(0);
		m_posbar->setValue (0);
		m_posbar->hide ();
		m_stereo->setStereoMono (false, false);
	}
}

void
MainDisplay::setPlaytime (uint32_t time)
{
	uint32_t showtime;
	if (m_mw->isTimemodeReverse()) {
		uint maxtime = m_posbar->maximum ();
		showtime = -(maxtime - time); 
	} else {
		showtime = time;
	}
	m_time->setTime (showtime);

	// update slider
	m_posbar->setValue (time);
}

void
MainDisplay::setMediainfo (const Xmms::PropDict &info)
{
	QString n;
	if (info.contains ("title")) {
		if (info.contains ("artist")) {
			n = QString::fromUtf8 (info.get<std::string> ("artist").c_str ()) + " - ";
		}
		if (info.contains ("album")) {
			n += QString::fromUtf8 (info.get<std::string> ("album").c_str ()) + " - ";
		}
		n += QString::fromUtf8 (info.get<std::string> ("title").c_str ());
	} else if (info.contains ("channel")) {
		n = QString::fromUtf8 (info.get<std::string> ("channel").c_str ()) + " - " +
		    QString::fromUtf8 (info.get<std::string> ("title").c_str ());
	} else {
		n = QString::fromUtf8 (info.get<std::string> ("url").c_str ());
		n = n.section ("/", -1);
	}
	m_text->setText (n);
	
	if (info.contains ("bitrate")) {
		m_kbps->setNumber (info.get<int32_t> ("bitrate")/1000, 3);
	} else {
		m_kbps->setNumber (0, 1);
	}

	if (info.contains ("samplerate")) {
		m_khz->setNumber (info.get<int32_t> ("samplerate")/1000, 2);
	} else {
		m_khz->setNumber(0, 1);
	}
	if (info.contains ("channels") &&
	    info.get<int32_t> ("channels") > 1) {
		m_stereo->setStereoMono (1, 0);
	} else {
		m_stereo->setStereoMono (0, 1);
	}

	if (info.contains ("duration")) {
		m_posbar->setMaximum (info.get<int32_t> ("duration"));
		m_posbar->show ();
	} else {
		m_posbar->hide ();
	}
}

void
MainDisplay::SetupToggleButtons (void)
{
	QSettings s;


	m_eq = new ToggleButton (this, Skin::EQ_ON_0, Skin::EQ_ON_1,
							 Skin::EQ_OFF_0, Skin::EQ_OFF_1);
	m_eq->move(219, 58);
	m_eq->setChecked (m_mw->getEQ ()->isVisible ());
	connect (m_eq, SIGNAL (toggled (bool)),
	         m_mw->getEQ (), SLOT (setVisible (bool)));
	connect (m_mw->getEQ (), SIGNAL (visibilityChanged (bool)),
	         m_eq, SLOT (setChecked (bool)));


	m_pls = new ToggleButton (this, Skin::PLS_ON_0, Skin::PLS_ON_1,
							  Skin::PLS_OFF_0, Skin::PLS_OFF_1);
	m_pls->move(242, 58);
	m_pls->setChecked (m_mw->getPL ()->isVisible ());
	connect (m_pls, SIGNAL (toggled (bool)),
	         m_mw->getPL (), SLOT (setVisible (bool)));
	connect (m_mw->getPL (), SIGNAL (visibilityChanged (bool)),
	         m_pls, SLOT (setChecked (bool)));


	m_shuffle = new ToggleButton (this, Skin::SHUFFLE_ON_0, Skin::SHUFFLE_ON_1,
								  Skin::SHUFFLE_OFF_0, Skin::SHUFFLE_OFF_1);
	m_shuffle->move(164, 89);
	m_shuffle->setEnabled(false); // FIXME: Disabled button for now, not yet implemented

	m_repeat = new ToggleButton (this, Skin::REPEAT_ON_0, Skin::REPEAT_ON_1,
								 Skin::REPEAT_OFF_0, Skin::REPEAT_OFF_1);
	m_repeat->move(210, 89);
	connect (m_repeat, SIGNAL (clicked (bool)),
	         this, SLOT (setRepeatAllEnabled (bool)));
}


void
MainDisplay::toggleTime (void)
{
	m_mw->setTimemodeReverse (!m_mw->isTimemodeReverse());
}

void
MainDisplay::SetupPushButtons (void)
{
	XMMSHandler &client = XMMSHandler::getInstance ();

	/* Normal buttons */
	m_prev = new Button (this, Skin::BTN_PREV_0, Skin::BTN_PREV_1);
	m_prev->move(16, 88);
	connect (m_prev, SIGNAL(clicked()), client.xplayback (), SLOT(prev ()));
	
	m_play = new Button (this, Skin::BTN_PLAY_0, Skin::BTN_PLAY_1);
	m_play->move(39, 88);
	connect (m_play, SIGNAL(clicked()), client.xplayback (), SLOT(play ()));

	m_pause = new Button (this, Skin::BTN_PAUSE_0, Skin::BTN_PAUSE_1);
	m_pause->move(62, 88);
	connect (m_pause, SIGNAL(clicked()), client.xplayback (), SLOT(pause ()));

	m_stop = new Button (this, Skin::BTN_STOP_0, Skin::BTN_STOP_1);
	m_stop->move(85, 88);
	connect (m_stop, SIGNAL(clicked()), client.xplayback (), SLOT(stop ()));

	m_next = new Button (this, Skin::BTN_NEXT_0, Skin::BTN_NEXT_1);
	m_next->move(108, 88);
	connect (m_next, SIGNAL(clicked()), client.xplayback (), SLOT(next ()));

	m_eject = new Button (this, Skin::BTN_EJECT_0, Skin::BTN_EJECT_1);
	m_eject->move(136, 89);
	connect (m_eject, SIGNAL(clicked()), this, SLOT(fileOpen()));

}


/*
 * Methods for interaction with the server configuration
 */
void
MainDisplay::setupServerConfig ()
{
	connect (m_xconfig, SIGNAL (configChanged (QString, QString)),
	         this, SLOT (serverConfigChanged (QString, QString)));

	if (m_xconfig->isReady ()) {
		QString key;
		QString value;
		// set repeat button
		key = QString ("playlist.repeat_all");
		value = m_xconfig->value_get (key);
		serverConfigChanged (key, value);
	}
}

void
MainDisplay::serverConfigChanged (QString key, QString value)
{
	if (key == "playlist.repeat_all") {
		if (value != "0") {
			m_repeat->setChecked (true);
		} else {
			m_repeat->setChecked (false);
		}
	}
}

void
MainDisplay::setRepeatAllEnabled (bool enabled) {
	if (enabled) {
		m_xconfig->value_set ("playlist.repeat_all", "1");
	} else {
		m_xconfig->value_set ("playlist.repeat_all", "0");
	}
}

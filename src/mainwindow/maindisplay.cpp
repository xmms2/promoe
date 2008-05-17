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

#include "pixmapbutton.h"
#include "pixmapslider.h"
#include "TitleBar.h"
#include "TextBar.h"
#include "NumberDisplay.h"
#include "TimeDisplay.h"
#include "Skin.h"
#include "SmallNumberDisplay.h"
#include "stereomono.h"
#include "posbar.h"
#include "playstatus.h"
#include "playlistwindow.h"
#include "equalizerwindow.h"
#include "clutterbar.h"

#include <QFileDialog>
#include <QSettings>
#include <QDebug>

MainDisplay::MainDisplay (QWidget *parent) : SkinDisplay(parent)
{
	XMMSHandler &client = XMMSHandler::getInstance ();
	m_xconfig = client.xconfig ();
	Skin* skin = Skin::getInstance ();

	m_tbar = new TitleBar(this, false);
	m_tbar->move(0, 0);
	m_tbar->resize(275, 14);

	m_mw = dynamic_cast<MainWindow *>(parent);

	SetupPushButtons ();
	SetupToggleButtons ();

	m_text = new TextScroller (this, 154, 10, "main");
	m_text->move (112, 25);

	m_time = new TimeDisplay(this, 0);
//	m_time->move (36, 26);
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
	m_posbar->move (skin->getPos (Skin::SLIDER_POSBAR_BGS));

	m_playstatus = new PlayStatus (this);
	m_playstatus->move (24, 28);

	m_vslider = new PixmapSlider(this);
	m_vslider->setMinimum (0);
	m_vslider->setMaximum (100);
	m_vslider->setSliderOffset (QPoint (0, 1));
	m_vslider->resize (skin->getSize (Skin::SLIDER_VOLUMEBAR_BGS));
	m_vslider->move (skin->getPos (Skin::SLIDER_VOLUMEBAR_BGS));

	m_bslider = new PixmapSlider (this);
	m_bslider->setMinimum (-20);
	m_bslider->setMaximum (20);
	m_bslider->setSliderOffset (QPoint (0, 1));
	m_bslider->resize (skin->getSize (Skin::SLIDER_BALANCEBAR_BGS));
	m_bslider->move (skin->getPos (Skin::SLIDER_BALANCEBAR_BGS));

	connect (&client, SIGNAL(currentSong (const Xmms::PropDict &)), 
			 this, SLOT(setMediainfo (const Xmms::PropDict &)));
	connect (client.xplayback (), SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(setStatus(Xmms::Playback::Status)));
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

	/* update buttons*/
	m_prev->setIcon (skin->getIcon (Skin::BUTTON_MW_PREV));
	m_play->setIcon (skin->getIcon (Skin::BUTTON_MW_PLAY));
	m_pause->setIcon (skin->getIcon (Skin::BUTTON_MW_PAUSE));
	m_stop->setIcon (skin->getIcon (Skin::BUTTON_MW_STOP));
	m_next->setIcon (skin->getIcon (Skin::BUTTON_MW_NEXT));
	m_eject->setIcon (skin->getIcon (Skin::BUTTON_MW_EJECT));
	m_eq->setIcon (skin->getIcon (Skin::BUTTON_MW_EQ));
	m_pls->setIcon (skin->getIcon (Skin::BUTTON_MW_PLS));
	m_shuffle->setIcon (skin->getIcon (Skin::BUTTON_MW_SHUFFLE));
	m_repeat->setIcon (skin->getIcon (Skin::BUTTON_MW_REPEAT));

	/* update Sliders */
	m_vslider->setBackground (skin->getBackgrounds (Skin::SLIDER_VOLUMEBAR_BGS));
	m_vslider->setSliders (skin->getItem (Skin::VOLBAR_BTN_0),
	                       skin->getItem (Skin::VOLBAR_BTN_1));
	m_bslider->setBackground (skin->getBackgrounds (Skin::SLIDER_BALANCEBAR_BGS));
	m_bslider->setSliders (skin->getItem (Skin::BALANCE_BTN_0),
	                       skin->getItem (Skin::BALANCE_BTN_1));
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
	Skin *skin = Skin::getInstance ();

	m_eq = new PixmapButton (this);
	m_eq->setCheckable (true);
	m_eq->resize (skin->getSize (Skin::BUTTON_MW_EQ));
	m_eq->move (skin->getPos (Skin::BUTTON_MW_EQ));
	m_eq->setChecked (m_mw->getEQ ()->isVisible ());
	connect (m_eq, SIGNAL (toggled (bool)),
	         m_mw->getEQ (), SLOT (setVisible (bool)));
	connect (m_mw->getEQ (), SIGNAL (visibilityChanged (bool)),
	         m_eq, SLOT (setChecked (bool)));

	m_pls = new PixmapButton (this);
	m_pls->setCheckable (true);
	m_pls->resize (skin->getSize (Skin::BUTTON_MW_PLS));
	m_pls->move (skin->getPos (Skin::BUTTON_MW_PLS));
	m_pls->setChecked (m_mw->getPL ()->isVisible ());
	connect (m_pls, SIGNAL (toggled (bool)),
	         m_mw->getPL (), SLOT (setVisible (bool)));
	connect (m_mw->getPL (), SIGNAL (visibilityChanged (bool)),
	         m_pls, SLOT (setChecked (bool)));

	// FIXME: Shuffle not yet implemented
	// Shuffled play should be done serverside through a service client
	m_shuffle = new PixmapButton (this);
	m_shuffle->setCheckable (true);
	m_shuffle->resize (skin->getSize (Skin::BUTTON_MW_SHUFFLE));
	m_shuffle->move (skin->getPos (Skin::BUTTON_MW_SHUFFLE));
	m_shuffle->setEnabled(false); 	

	m_repeat = new PixmapButton (this);
	m_repeat->setCheckable (true);
	m_repeat->resize (skin->getSize (Skin::BUTTON_MW_REPEAT));
	m_repeat->move (skin->getPos (Skin::BUTTON_MW_REPEAT));
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
	Skin *skin = Skin::getInstance ();

	/* Normal buttons */
	m_prev = new PixmapButton (this);
	m_prev->resize (skin->getSize (Skin::BUTTON_MW_PREV));
	m_prev->move (skin->getPos (Skin::BUTTON_MW_PREV));
	connect (m_prev, SIGNAL(clicked()), client.xplayback (), SLOT(prev ()));
	
	m_play = new PixmapButton (this);
	m_play->resize (skin->getSize (Skin::BUTTON_MW_PLAY));
	m_play->move (skin->getPos (Skin::BUTTON_MW_PLAY));
	connect (m_play, SIGNAL(clicked()), client.xplayback (), SLOT(play ()));

	m_pause = new PixmapButton (this);
	m_pause->resize (skin->getSize (Skin::BUTTON_MW_PAUSE));
	m_pause->move (skin->getPos (Skin::BUTTON_MW_PAUSE));
	connect (m_pause, SIGNAL(clicked()),
	         client.xplayback (), SLOT(toggle_pause ()));

	m_stop = new PixmapButton (this);
	m_stop->resize (skin->getSize (Skin::BUTTON_MW_STOP));
	m_stop->move (skin->getPos (Skin::BUTTON_MW_STOP));
	connect (m_stop, SIGNAL(clicked()), client.xplayback (), SLOT(stop ()));

	m_next = new PixmapButton (this);
	m_next->resize (skin->getSize (Skin::BUTTON_MW_NEXT));
	m_next->move (skin->getPos (Skin::BUTTON_MW_NEXT));
	connect (m_next, SIGNAL(clicked()), client.xplayback (), SLOT(next ()));

	m_eject = new PixmapButton (this);
	m_eject->resize (skin->getSize (Skin::BUTTON_MW_EJECT));
	m_eject->move (skin->getPos (Skin::BUTTON_MW_EJECT));
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

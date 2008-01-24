#include <xmmsclient/xmmsclient++.h>
#include "XMMSHandler.h"
#include "MainDisplay.h"
#include "MainWindow.h"

#include "TitleBar.h"
#include "Button.h"
#include "TextBar.h"
#include "NumberDisplay.h"
#include "TimeDisplay.h"
#include "SmallNumberDisplay.h"
#include "StereoMono.h"
#include "PosBar.h"
#include "PlayStatus.h"
#include "VolumeSlider.h"
#include "playlist.h"
#include "ClutterBar.h"

#include <QFileDialog>
#include <QSettings>

MainDisplay::MainDisplay (QWidget *parent) : SkinDisplay(parent)
{
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

	XMMSHandler &xmmsh = XMMSHandler::getInstance ();
	connect (&xmmsh, SIGNAL(currentSong (const Xmms::PropDict &)), 
			 this, SLOT(setMediainfo (const Xmms::PropDict &)));
	connect (&xmmsh, SIGNAL(playbackStatusChanged(Xmms::Playback::Status)),
	         this, SLOT(setStatus(Xmms::Playback::Status)));
//	connect (&xmmsh, SIGNAL(playtimeChanged(uint)),
//	         this, SLOT(setPlaytime(uint)));
	connect (xmmsh.cache () , SIGNAL (playtime (uint32_t)),
	         this,  SLOT (setPlaytime (uint32_t)));
	connect (&xmmsh, SIGNAL(getVolume(uint)), this, SLOT(updateVolume(uint)));
	connect (m_vslider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));
	xmmsh.volumeGet();
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
MainDisplay::mouseMoveEvent (QMouseEvent *event)
{
	MainWindow *mw = dynamic_cast<MainWindow *>(window ());
	PlaylistWindow *pl = mw->getPL ();

	m_mw->move (event->globalPos().x() - m_diffX,
				event->globalPos().y() - m_diffY);

	if (!pl || !pl->isVisible ())
		return;

	pl->move (event->globalPos().x() - m_diffX,
			  event->globalPos().y() - m_diffY + size().height());
}

void
MainDisplay::setPixmaps (Skin *skin)
{
	QPalette palette = QPalette();
	QBrush brush = QBrush(skin->getItem(Skin::MAIN_WINDOW));
	palette.setBrush(QPalette::Background, brush);
	setPalette(palette);

	setMaximumSize(QSize(275, 116));
	setMinimumSize(QSize(275, 116));
}

void
MainDisplay::setStatus (Xmms::Playback::Status status)
{
	if (status == Xmms::Playback::STOPPED) {
		m_time->setTime(0);
		m_posbar->setPos (0);
		m_posbar->hideBar (true);
	}
}

void
MainDisplay::setPlaytime (uint32_t time)
{
	uint32_t showtime;
	if (m_mw->isTimemodeReverse()) {
		uint maxtime = m_posbar->getMax();
		showtime = -(maxtime - time); 
	} else {
		showtime = time;
	}
	m_time->setTime (showtime);

	// update slider
	m_posbar->setPos (time);
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
	if (info.contains ("channels:in") && 
	    info.get<int32_t> ("channels:in") > 1) {
		m_stereo->setStereoMono (1, 0);
	} else {
		m_stereo->setStereoMono (0, 1);
	}

	if (info.contains ("duration")) {
		m_posbar->setMax (info.get<int32_t> ("duration"));
		m_posbar->hideBar (false);
	} else {
		m_posbar->hideBar (true);
	}
}

void
MainDisplay::SetupToggleButtons (void)
{
	QSettings s;

	m_pls = new ToggleButton (this, Skin::PLS_ON_0, Skin::PLS_ON_1,
							  Skin::PLS_OFF_0, Skin::PLS_OFF_1);
	m_pls->move(242, 58);
	if (!s.value ("playlist/hidden").toBool ())
		m_pls->toggleOn ();

	connect (m_pls, SIGNAL(clicked()), this, SLOT(togglePL()));

	m_eq = new ToggleButton (this, Skin::EQ_ON_0, Skin::EQ_ON_1,
							 Skin::EQ_OFF_0, Skin::EQ_OFF_1);
	m_eq->move(219, 58);
	if (!s.value ("equalizer/hidden").toBool ())
		m_pls->toggleOn ();

	connect (m_eq, SIGNAL(clicked()), this, SLOT(toggleEQ()));

	m_shuffle = new ToggleButton (this, Skin::SHUFFLE_ON_0, Skin::SHUFFLE_ON_1,
								  Skin::SHUFFLE_OFF_0, Skin::SHUFFLE_OFF_1);
	m_shuffle->move(164, 89);

	m_repeat = new ToggleButton (this, Skin::REPEAT_ON_0, Skin::REPEAT_ON_1,
								 Skin::REPEAT_OFF_0, Skin::REPEAT_OFF_1);
	m_repeat->move(210, 89);
}

void
MainDisplay::togglePL (void)
{
	m_mw->togglePL(false);
}

void
MainDisplay::toggleEQ (void)
{
	m_mw->toggleEQ(false);
}

void
MainDisplay::toggleTime (void)
{
	m_mw->setTimemodeReverse (!m_mw->isTimemodeReverse());
}

void
MainDisplay::SetupPushButtons (void)
{
	XMMSHandler &xmmsh = XMMSHandler::getInstance ();

	/* Normal buttons */
	m_prev = new Button (this, Skin::BTN_PREV_0, Skin::BTN_PREV_1);
	m_prev->move(16, 88);
	connect (m_prev, SIGNAL(clicked()), &xmmsh, SLOT(prev()));
	
	m_play = new Button (this, Skin::BTN_PLAY_0, Skin::BTN_PLAY_1);
	m_play->move(39, 88);
	connect (m_play, SIGNAL(clicked()), &xmmsh, SLOT(play()));

	m_pause = new Button (this, Skin::BTN_PAUSE_0, Skin::BTN_PAUSE_1);
	m_pause->move(62, 88);
	connect (m_pause, SIGNAL(clicked()), &xmmsh, SLOT(pause()));

	m_stop = new Button (this, Skin::BTN_STOP_0, Skin::BTN_STOP_1);
	m_stop->move(85, 88);
	connect (m_stop, SIGNAL(clicked()), &xmmsh, SLOT(stop()));

	m_next = new Button (this, Skin::BTN_NEXT_0, Skin::BTN_NEXT_1);
	m_next->move(108, 88);
	connect (m_next, SIGNAL(clicked()), &xmmsh, SLOT(next()));

	m_eject = new Button (this, Skin::BTN_EJECT_0, Skin::BTN_EJECT_1);
	m_eject->move(136, 89);
	connect (m_eject, SIGNAL(clicked()), this, SLOT(fileOpen()));

}

MainDisplay::~MainDisplay (void)
{
}

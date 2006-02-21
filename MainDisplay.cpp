#include "XMMSHandler.h"
#include "MainDisplay.h"
#include "TitleBar.h"
#include "TextBar.h"

MainDisplay::MainDisplay (QWidget *parent) : SkinDisplay(parent)
{
	m_mw = dynamic_cast<MainWindow*>(parent);

	m_tbar = new TitleBar(this, false);
	m_tbar->move(0, 0);
	m_tbar->resize(275, 14);

	SetupPushButtons ();
	SetupToggleButtons ();

	m_text = new TextScroller (this, 154, 15);
	m_text->move (109, 23);

	m_number = new NumberDisplay (this, 33, 11);
	m_number->move (37, 26);

	m_number2 = new NumberDisplay (this, 24, 0);
	m_number2->move (78, 26);

	m_kbps = new SmallNumberDisplay (this, 15);
	m_kbps->move (111, 43);
	m_kbps->setNumber (128, 3);

	m_khz = new SmallNumberDisplay (this, 10);
	m_khz->move (156, 43);
	m_khz->setNumber (44, 2);

	m_stereo = new StereoMono (this);
	m_stereo->move (212, 41);
	m_stereo->setStereoMono (0, 0);

	m_slider = new Slider (this, Skin::POSBAR, 
						   Skin::POSBAR_BTN_0, 
						   Skin::POSBAR_BTN_1);
	m_slider->move (16, 72);

	m_playstatus = new PlayStatus (this);
	m_playstatus->move (24, 28);

}

void
MainDisplay::setPixmaps (Skin *skin)
{
	QPalette palette = QPalette();
	QBrush brush = QBrush(Qt::TexturePattern);
	brush.setTexture(skin->getItem(Skin::MAIN_WINDOW));
	palette.setBrush(QPalette::Background, brush);
	setPalette(palette);

	setMaximumSize(QSize(275, 116));
	setMinimumSize(QSize(275, 116));
}

void
MainDisplay::SetupToggleButtons (void)
{
	m_pls = new ToggleButton (this, Skin::PLS_ON_0, Skin::PLS_ON_1,
							  Skin::PLS_OFF_0, Skin::PLS_OFF_1);
	m_pls->move(242, 58);

	m_eq = new ToggleButton (this, Skin::EQ_ON_0, Skin::EQ_ON_1,
							 Skin::EQ_OFF_0, Skin::EQ_OFF_1);
	m_eq->move(219, 58);

	m_shuffle = new ToggleButton (this, Skin::SHUFFLE_ON_0, Skin::SHUFFLE_ON_1,
								  Skin::SHUFFLE_OFF_0, Skin::SHUFFLE_OFF_1);
	m_shuffle->move(164, 89);

	m_repeat = new ToggleButton (this, Skin::REPEAT_ON_0, Skin::REPEAT_ON_1,
								 Skin::REPEAT_OFF_0, Skin::REPEAT_OFF_1);
	m_repeat->move(210, 89);
}

void
MainDisplay::SetupPushButtons (void)
{
	MainWindow *mw = dynamic_cast<MainWindow *>(window ());

	/* Normal buttons */
	m_prev = new Button (this, Skin::BTN_PREV_0, Skin::BTN_PREV_1);
	m_prev->move(16, 88);
	connect (m_prev, SIGNAL(clicked()), mw->getHandler (), SLOT(prev()));
	
	m_play = new Button (this, Skin::BTN_PLAY_0, Skin::BTN_PLAY_1);
	m_play->move(39, 88);
	connect (m_play, SIGNAL(clicked()), mw->getHandler (), SLOT(play()));

	m_pause = new Button (this, Skin::BTN_PAUSE_0, Skin::BTN_PAUSE_1);
	m_pause->move(62, 88);
	connect (m_pause, SIGNAL(clicked()), mw->getHandler (), SLOT(pause()));

	m_stop = new Button (this, Skin::BTN_STOP_0, Skin::BTN_STOP_1);
	m_stop->move(85, 88);
	connect (m_stop, SIGNAL(clicked()), mw->getHandler (), SLOT(stop()));

	m_next = new Button (this, Skin::BTN_NEXT_0, Skin::BTN_NEXT_1);
	m_next->move(108, 88);
	connect (m_next, SIGNAL(clicked()), mw->getHandler (), SLOT(next()));

	m_eject = new Button (this, Skin::BTN_EJECT_0, Skin::BTN_EJECT_1);
	m_eject->move(136, 89);

}

MainDisplay::~MainDisplay (void)
{
}

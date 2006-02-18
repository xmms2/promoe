#include "MainDisplay.h"
#include "TitleBar.h"
#include "TextBar.h"

MainDisplay::MainDisplay (QWidget *parent) : SkinDisplay(parent)
{
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
	/* Normal buttons */
	m_prev = new Button (this, Skin::BTN_PREV_0, Skin::BTN_PREV_1);
	m_prev->move(16, 88);
	m_play = new Button (this, Skin::BTN_PLAY_0, Skin::BTN_PLAY_1);
	m_play->move(39, 88);
	m_pause = new Button (this, Skin::BTN_PAUSE_0, Skin::BTN_PAUSE_1);
	m_pause->move(62, 88);
	m_stop = new Button (this, Skin::BTN_STOP_0, Skin::BTN_STOP_1);
	m_stop->move(85, 88);
	m_next = new Button (this, Skin::BTN_NEXT_0, Skin::BTN_NEXT_1);
	m_next->move(108, 88);
	m_eject = new Button (this, Skin::BTN_EJECT_0, Skin::BTN_EJECT_1);
	m_eject->move(136, 89);

	m_menubtn = new Button (this, Skin::MENUBUTTON_0, Skin::MENUBUTTON_1);
	m_menubtn->move(6, 3);
	m_minimize = new Button (this, Skin::MINIMIZE_0, Skin::MINIMIZE_1);
	m_minimize->move(244, 3);
	m_shadebtn = new Button (this, Skin::SHADE_1_0, Skin::SHADE_1_1);
	m_shadebtn->move(254, 3);
	m_closebtn = new Button (this, Skin::CLOSE_0, Skin::CLOSE_1);
	m_closebtn->move(264, 3);
}

MainDisplay::~MainDisplay (void)
{
}

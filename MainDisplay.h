#ifndef __MAINDISPLAY_H__
#define __MAINDISPLAY_H__

#include <iostream>

#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QHash>
#include <QBrush>
#include <QPalette>

#include "Display.h"
#include "TitleBar.h"
#include "Button.h"
#include "TextBar.h"
#include "MainWindow.h"
#include "NumberDisplay.h"

using namespace std;

class MainDisplay : public SkinDisplay 
{
	Q_OBJECT
	public:
		MainDisplay (QWidget *parent);
		~MainDisplay ();
	protected:
		void SetupPushButtons (void);
		void MainDisplay::SetupToggleButtons (void);

		Button *m_prev;
		Button *m_play;
		Button *m_pause;
		Button *m_stop;
		Button *m_next;
		Button *m_eject;
		Button *m_shadebtn;
		Button *m_menubtn;
		Button *m_closebtn;
		Button *m_minimize;

		ToggleButton *m_pls;
		ToggleButton *m_eq;
		ToggleButton *m_shuffle;
		ToggleButton *m_repeat;

		TextScroller *m_text;

		NumberDisplay *m_number;
		NumberDisplay *m_number2;
};

#endif

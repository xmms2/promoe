#ifndef __MAINDISPLAY_H__
#define __MAINDISPLAY_H__

class MainDisplay;

#include "XMMSHandler.h"

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
#include "NumberDisplay.h"
#include "SmallNumberDisplay.h"
#include "StereoMono.h"
#include "Slider.h"
#include "PlayStatus.h"
#include "MainWindow.h"

class MainDisplay : public SkinDisplay 
{
	Q_OBJECT
	public:
		MainDisplay (QWidget *parent);
		~MainDisplay ();
		ToggleButton *GetPls() {return m_pls;};

		TextScroller *m_text;
		NumberDisplay *m_number;
		NumberDisplay *m_number2;

		SmallNumberDisplay *m_kbps;
		SmallNumberDisplay *m_khz;

		StereoMono *m_stereo;
		Slider *m_slider;

		PlayStatus *m_playstatus;

	public slots:
		void setPixmaps(Skin *skin);
		void setStatus (uint status);
		void setPlaytime (uint time);
		void setMediainfo (QHash<QString,QString>);
		void togglePL(void);

	protected:
		void SetupPushButtons (void);
		void SetupToggleButtons (void);

		Button *m_prev;
		Button *m_play;
		Button *m_pause;
		Button *m_stop;
		Button *m_next;
		Button *m_eject;

		ToggleButton *m_pls;
		ToggleButton *m_eq;
		ToggleButton *m_shuffle;
		ToggleButton *m_repeat;

		MainWindow *m_mw;

};

#endif

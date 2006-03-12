#ifndef __SHADEDDISPLAY_H__
#define __SHADEDDISPLAY_H__

class ShadedDisplay;

#include "MainWindow.h"
#include "Display.h"
#include "SmallNumberDisplay.h"
#include "TextBar.h"
#include "Button.h"

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
		Button *m_prev;
		Button *m_play;
		Button *m_pause;
		Button *m_stop;
		Button *m_next;
		Button *m_eject;

	public slots:
		void setStatus (uint status);
		void setPlaytime (uint time);
		void setMediainfo (const QHash<QString, QString> &h);
};

#endif

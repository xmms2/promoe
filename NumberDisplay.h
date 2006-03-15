#ifndef __NUMBERDISPLAY_H__
#define __NUMBERDISPLAY_H__

class TimeDisplay;

#include "PixWidget.h"
//#include "TimeDisplay.h"

class NumberDisplay : public PixWidget
{
	Q_OBJECT
	public:
		NumberDisplay (QWidget *parent, TimeDisplay *td, uint, uint);
		~NumberDisplay ();

		void setNumber (uint, uint);

	public slots:
		void setPixmaps (Skin *skin);
	signals:
		void clicked(void);

	protected:
		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);
		int m_w;
		int m_startpx;

		TimeDisplay *m_td;
		uint m_n1;
		uint m_n2;
};


#endif

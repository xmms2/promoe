#ifndef __TIMEDISPLAY_H__
#define __TIMEDISPLAY_H__

#include "PixWidget.h"
#include "NumberDisplay.h"

class TimeDisplay : public PixWidget
{
	Q_OBJECT
	public:
		TimeDisplay (QWidget *parent, int time);
		~TimeDisplay ();
		void setTime (int);
	public slots:
		void setPixmaps (Skin *skin);
	signals:
		void clicked(void);

	protected:
		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);
		void drawMinus();
		int m_time;
		NumberDisplay *m_number_min;
		NumberDisplay *m_number_sec;

		uint m_w;
};

#endif

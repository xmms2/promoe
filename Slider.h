#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "PixWidget.h"
#include "Button.h"

class Slider;

class BarButton : public Button
{
	public:
		BarButton (QWidget *, uint, uint);

		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);
		void mouseReleaseEvent (QMouseEvent *);

		void setPos (uint pos);
		uint getPos (void);

	private:
		Slider *m_slider;
		bool m_moving;
		uint m_pos;
};

class Slider : public PixWidget
{
	Q_OBJECT

	public:
		Slider (QWidget *parent, uint bg, uint bnormal, uint bpressed, bool vertical=true);
		~Slider () { }

		void setSize (uint, uint);
		uint getPos (void);
		void setPos (uint);
		void requestPos (float value);
		
		void setMax (uint max) { m_max = max; }
		uint getMax () { return m_max; }
		void hideBar (bool b); 		
		bool getVertical () { return m_vertical; }

	public slots:
		void setPixmaps (Skin *skin);

	private:
		bool m_vertical;
		int m_bg;
		uint m_max;
		uint m_pix;
		BarButton *m_button;

};

#endif

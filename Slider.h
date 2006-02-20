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

	private:
		Slider *m_slider;
	

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
		
		void setMax (uint max) { m_max = max; }
		void hideBar (bool b); 		
		bool getVertical () { return m_vertical; }

	public slots:
		void setPixmaps (Skin *skin);

	private:
		bool m_vertical;
		int m_bg;
		uint m_max;
		uint m_pix;
		uint m_pos;
		BarButton *m_button;

};

#endif

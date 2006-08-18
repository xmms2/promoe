#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "PixWidget.h"
#include "Button.h"

class PosBar;

class PosButton : public Button
{
	public:
		PosButton (QWidget *, uint, uint);

		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);
		void mouseReleaseEvent (QMouseEvent *);

		void setPos (uint pos);
		uint getPos (void);

	private:
		PosBar *m_slider;
		bool m_moving;
		uint m_pos;
};

class PosBar : public PixWidget
{
	Q_OBJECT

	public:
		PosBar (QWidget *parent, uint bg, uint bnormal, uint bpressed, bool vertical=true);
		~PosBar () { }

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
		PosButton *m_button;

};

#endif

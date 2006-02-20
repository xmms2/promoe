#ifndef __NUMBERDISPLAY_H__
#define __NUMBERDISPLAY_H__

#include "PixWidget.h"

class NumberDisplay : public PixWidget
{
	Q_OBJECT
	public:
		NumberDisplay (QWidget *parent, uint, uint);
		~NumberDisplay ();

		void setNumber (uint, uint);

	public slots:
		void setPixmaps (Skin *skin);

	protected:
		int m_w;
		int m_startpx;

		uint m_n1;
		uint m_n2;
};


#endif

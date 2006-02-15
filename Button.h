#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <QWidget>
#include "PixWidget.h"

using namespace std;

class Button : public PixWidget
{
	Q_OBJECT
	public:
		Button (QWidget *parent, uint btn1, uint btn2);
		~Button ();
		void setFunc (void (*func)(void *userdata), void *userdata) { m_func = func; m_userdata = userdata; }

	protected:
		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);

		QPixmap m_pixmap_normal;
		QPixmap m_pixmap_pressed;
		void (*m_func)(void *userdata);
		void *m_userdata;
};

class ToggleButton : public Button
{
	Q_OBJECT
	public:
		ToggleButton (QWidget *parent, uint, uint, uint, uint);
		~ToggleButton ();

		void toggleOn (bool);
		void setFunc (void (*func)(void *userdata), void *userdata) { m_func2 = func; m_userdata2 = userdata; }

		bool getOn () const { return m_toggled_on; }
		void runFunc ();

	private:
		void (*m_func2)(void *userdata);
		void *m_userdata2;
		
		QPixmap m_pixmap_on_normal;
		QPixmap m_pixmap_on_pressed;
		QPixmap m_pixmap_off_normal;
		QPixmap m_pixmap_off_pressed;

		bool m_toggled_on;
};

#endif

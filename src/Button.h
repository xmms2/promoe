#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "PixWidget.h"

class Button : public PixWidget
{
	Q_OBJECT
	public:
		Button (QWidget *parent, uint btn1, uint btn2, bool pls=false);
		Button (QWidget *parent);
		~Button ();

	public slots:
		void setPixmaps (Skin *skin);

	signals:
		void clicked (void);

	protected:
		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);
		void mouseMoveEvent (QMouseEvent *event) {}

		uint m_name_normal;
		uint m_name_pressed;

		uint m_diffx;
		uint m_diffy;
		
		QPixmap m_pixmap_normal;
		QPixmap m_pixmap_pressed;

		bool m_nodrag;
		bool m_pls;
};

class ToggleButton : public Button
{
	Q_OBJECT
	public:
		ToggleButton (QWidget *parent, uint, uint, uint, uint);
		~ToggleButton ();

		bool getOn () const { return m_toggled_on; }

	public slots:
		void setPixmaps(Skin *skin);
		void toggleOn ();

	private:
		uint m_name_on_normal;
		uint m_name_on_pressed;
		uint m_name_off_normal;
		uint m_name_off_pressed;
		
		QPixmap m_pixmap_on_normal;
		QPixmap m_pixmap_on_pressed;
		QPixmap m_pixmap_off_normal;
		QPixmap m_pixmap_off_pressed;

		bool m_toggled_on;
		
		void setCurrentPix ();
};

#endif

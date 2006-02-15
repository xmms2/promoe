#ifndef __TITLEBAR_H__
#define __TITLEBAR_H__

#include <QWidget>
#include "PixWidget.h"

using namespace std;

class TitleBar : public PixWidget
{
	Q_OBJECT
	public:
		TitleBar (QWidget *parent, bool shaded);
		~TitleBar ();
		void setActive (bool active);
	protected:
		void mouseDoubleClickEvent (QMouseEvent *event);
	private:
		QPixmap m_pixmap_active;
		QPixmap m_pixmap_inactive;
};

#endif

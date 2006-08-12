#ifndef __CLUTTERBAR_H__
#define __CLUTTERBAR_H__

#include <QMouseEvent>
#include "PixWidget.h"

class ClutterBar : public PixWidget
{
	Q_OBJECT
	public:
		ClutterBar (QWidget *parent);
		~ClutterBar ();

	public slots:
		void setPixmaps(Skin *skin);

	protected:
		void mousePressEvent (QMouseEvent *event);
		void mouseReleaseEvent (QMouseEvent *event);

		QPixmap m_clutter_off;
		QPixmap m_clutter_on;

		QPixmap m_clutter_o;
		QPixmap m_clutter_a;
		QPixmap m_clutter_i;
		QPixmap m_clutter_d;
		QPixmap m_clutter_v;

		bool enabled;

		int m_ypos;
};


#endif

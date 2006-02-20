#ifndef __PIXWIDGET_H__
#define __PIXWIDGET_H__

#include <iostream>

#include "Skin.h"

#include <QPixmap>
#include <QPainter>
#include <QWidget>

class PixWidget : public QWidget
{
	Q_OBJECT
	public:
		PixWidget(QWidget *parent = 0);
		~PixWidget();
		void paintEvent (QPaintEvent *event);
		QWidget *getMW (void) { return m_mw; }
	public slots:
		virtual void setPixmaps(Skin *skin);
	protected:
		QPixmap m_pixmap;
		QWidget *m_mw;
};

#endif

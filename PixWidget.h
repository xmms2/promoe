#ifndef __PIXWIDGET_H__
#define __PIXWIDGET_H__

#include <iostream>

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
	protected:
		QPixmap m_pixmap;
};

#endif

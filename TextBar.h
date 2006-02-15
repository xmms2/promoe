#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

#include <iostream>

#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QFont>
#include <QTimer>
#include <QBrush>
#include <QPalette>
#include <QBitmap>

#include "Skin.h"

class TextScroller : public QWidget
{
	Q_OBJECT

	public:
		TextScroller (QWidget *parent, uint, uint);
		~TextScroller ();

		void setText(const QString &text);

	public slots:
		void addOffset ();

	protected:
		QPixmap m_pixmap;
		void paintEvent (QPaintEvent *event);

	private:
		int m_w;
		int m_h;
		int m_y;
		int m_x_off;
		int m_x2_off;

		QTimer *m_timer;

		Skin *m_skin;

		void drawBitmapFont (const QString &text);
		void drawQtFont (const QString &text);
};

#endif

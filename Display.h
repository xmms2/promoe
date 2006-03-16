#ifndef __SKINDISPLAY_H__
#define __SKINDISPLAY_H__

#include <iostream>

#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QHash>
#include <QMouseEvent>
#include <QPaintEvent>

#include "Skin.h"

class SkinDisplay : public QWidget
{
	Q_OBJECT
	public:
		SkinDisplay (QWidget *parent);
		QWidget *getMW () const { return m_mw; }

	public slots:
		virtual void setPixmaps(Skin *skin);
		void fileOpen (void);

	protected:
		void mousePressEvent (QMouseEvent *);
		void mouseMoveEvent (QMouseEvent *);
		void paintEvent (QPaintEvent *event);
		void SkinDisplay::enterEvent (QEvent *event);
		void SkinDisplay::leaveEvent (QEvent *event);

		QWidget *m_mw;
		QWidget *m_tbar;
		
		int m_diffX;
		int m_diffY;
};

#endif

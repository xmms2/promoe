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

using namespace std;

class SkinDisplay : public QWidget
{
	Q_OBJECT
	public:
		SkinDisplay (QWidget *parent);

		bool getNoDrag() const { return m_noDrag; }
		void setNoDrag(bool d) { m_noDrag = d; }

		QWidget *getMW() const { return m_mw; }
	public slots:
		virtual void setPixmaps(Skin *skin);

	protected:
		void mousePressEvent (QMouseEvent *);
		void mouseMoveEvent (QMouseEvent *);
		void paintEvent (QPaintEvent *event);
		void SkinDisplay::enterEvent (QEvent *event);
		void SkinDisplay::leaveEvent (QEvent *event);

		QWidget *m_mw;
		QWidget *m_tbar;
		
	private:
		bool m_noDrag;
		int m_diffX;
		int m_diffY;
};

#endif

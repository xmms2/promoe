
#include "Playlist.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QRect>

PlaylistScrollButton::PlaylistScrollButton (PlaylistScroller *parent, uint normal, uint pressed) : Button (parent, normal, pressed, true)
{
	m_slider = parent;
}

void
PlaylistScrollButton::mouseMoveEvent (QMouseEvent *event)
{
	QPoint p (event->pos ());

	int npos = pos().y()+p.y()-m_diffy;
	if (npos >= 0 && npos + rect().height() <= m_slider->rect().height()) {
		move (0, npos);
	} else if (npos < 0) {
		move (0, 0);
		npos = 0;
	} else if (npos + rect().height() > m_slider->rect().height()) {
		move (0, m_slider->rect().height()-rect().height());
		npos = m_slider->rect().height()-rect().height();
	}

	m_slider->doScroll (npos);
}

PlaylistScroller::PlaylistScroller (PlaylistWindow *parent) : QWidget (parent)
{
	m_pixmap = QPixmap(0,0);
	connect (parent->getSkin(), SIGNAL (skinChanged (Skin *)), this, SLOT (setPixmaps(Skin *)));
	m_button = new PlaylistScrollButton (this, Skin::PLS_SCROLL_0, Skin::PLS_SCROLL_1);
	m_button->move (0, 0);
}

int
PlaylistScroller::getMax (void)
{
	return rect().height()-m_button->rect().height();
}

void
PlaylistScroller::setPixmaps (Skin *skin)
{
	m_pixmap = skin->getPls (Skin::PLS_RFILL2_0);

}

void 
PlaylistScroller::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull ()) {
		return;
	}

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (rect (), m_pixmap, m_pixmap.rect ());
	paint.end ();
}

PlaylistWindow::PlaylistWindow (QWidget *parent, Skin *skin) : QMainWindow (parent)
{
	m_skin = skin;
	m_noDrag = false;

	setWindowFlags (Qt::FramelessWindowHint);
	connect (m_skin, SIGNAL (skinChanged (Skin *)), this, SLOT (setPixmaps(Skin *)));

	setActive (underMouse ());

	m_view = new PlaylistView (this);
	m_view->move (10, 20);
	m_view->resize (size().width()-30, size().height()-20-38);

	m_list = new PlaylistList (m_view);

	m_scroller = new PlaylistScroller (this);
	connect (m_scroller, SIGNAL(scrolled(int)), this, SLOT(doScroll (int)));

	setMinimumSize (275, 116);
	resize (275, 300);
}

void
PlaylistWindow::doScroll (int pos)
{
	int npos = ((float)pos) / (float)(m_scroller->getMax()) * float(m_list->height() - m_view->height());
	m_list->setOffset (npos);
	m_list->scroll (0, npos);
}

void
PlaylistWindow::resizeEvent (QResizeEvent *event)
{
	m_view->resize (size().width()-30, size().height()-20-38);
	m_list->resize (m_view->size().width(), m_view->size().height());

}

void
PlaylistWindow::mousePressEvent (QMouseEvent *event)
{
	m_diffx = event->pos ().x ();
	m_diffy = event->pos ().y ();
}

void
PlaylistWindow::mouseMoveEvent (QMouseEvent *event)
{
	if (m_noDrag)
		return;

	move (event->globalPos().x() - m_diffx,
		  event->globalPos().y() - m_diffy);

}

void
PlaylistWindow::setPixmaps (Skin *skin)
{
	m_skin = skin;
	setActive (m_active);
	resize (size().width(), size().height());
}

void
PlaylistWindow::enterEvent (QEvent *event)
{
	setActive (true);
}


void
PlaylistWindow::leaveEvent (QEvent *event)
{
	setActive (false);
}

void
PlaylistWindow::setActive (bool active)
{
	m_active = active;

	if (!m_skin) {
		return;
	}

	m_corner3 = m_skin->getPls (Skin::PLS_LCBAR);
	m_corner4 = m_skin->getPls (Skin::PLS_RCBAR);

	if (active) {
		m_corner1 = m_skin->getPls (Skin::PLS_CORNER_UL_0);
		m_corner2 = m_skin->getPls (Skin::PLS_CORNER_UR_0);
		m_titlebar = m_skin->getPls (Skin::PLS_TBAR_0);
		m_tfill = m_skin->getPls (Skin::PLS_TFILL_0);
		m_bfill = m_skin->getPls (Skin::PLS_BFILL_0);
		m_lfill = m_skin->getPls (Skin::PLS_LFILL_0);
		m_rfill = m_skin->getPls (Skin::PLS_RFILL_0);
		m_rfill2 = m_skin->getPls (Skin::PLS_RFILL2_0);
		m_rfill3 = m_skin->getPls (Skin::PLS_RFILL3_0);
	} else {
		m_corner1 = m_skin->getPls (Skin::PLS_CORNER_UL_1);
		m_corner2 = m_skin->getPls (Skin::PLS_CORNER_UR_1);
		m_titlebar = m_skin->getPls (Skin::PLS_TBAR_1);
		m_tfill = m_skin->getPls (Skin::PLS_TFILL_1);
		m_bfill = m_skin->getPls (Skin::PLS_BFILL_1);
		m_lfill = m_skin->getPls (Skin::PLS_LFILL_1);
		m_rfill = m_skin->getPls (Skin::PLS_RFILL_1);
		m_rfill2 = m_skin->getPls (Skin::PLS_RFILL2_1);
		m_rfill3 = m_skin->getPls (Skin::PLS_RFILL3_1);
	}

	update ();
}

void
PlaylistWindow::paintEvent (QPaintEvent *event)
{
	QPainter paint;

	paint.begin (this);
	paint.drawPixmap (QRect (0, 0,
							 m_corner1.size ().width (),
							 m_corner1.size ().height ()),
					  m_corner1,
					  m_corner1.rect ());

	paint.drawPixmap (QRect (size().width()-m_corner2.width(),
							 0, m_corner2.width(),
							 m_corner2.height()),
					  m_corner2,
					  m_corner2.rect());

	paint.drawPixmap (QRect (0, size().height()-m_corner3.height(),
							 m_corner3.width(),
							 m_corner3.height()),
					  m_corner3,
					  m_corner3.rect());

	paint.drawPixmap (QRect (size().width()-m_corner4.width(),
							 size().height()-m_corner4.height(),
							 m_corner4.width(),
							 m_corner4.height()),
					  m_corner4,
					  m_corner4.rect());

	int midx = (size().width()/2) - (m_titlebar.width()/2);
	paint.drawPixmap (QRect(midx, 0, m_titlebar.width(), m_titlebar.height()),
					  m_titlebar, m_titlebar.rect());

	/* left fill */
	paint.drawPixmap (QRect(m_corner1.width(),
							0, midx - m_corner1.width(),
							m_tfill.height()),
					  m_tfill,
					  m_tfill.rect());

	int midx2 = midx + m_titlebar.width();

	paint.drawPixmap (QRect (midx2, 0, size().width()-midx2-m_corner2.width(),
							 m_tfill.height()),
					  m_tfill,
					  m_tfill.rect());

	int bsize = m_corner3.width()+m_corner4.width();

	if (size().width() > bsize) {
		int pad_to = size().width() - bsize;
		paint.drawPixmap (QRect (m_corner3.width(),
								 size().height()-m_bfill.height(),
								 pad_to,
								 m_bfill.height()),
						  m_bfill,
						  m_bfill.rect());
	}

	paint.drawPixmap (QRect (0, m_corner1.height(), m_lfill.width(),
							 size().height()-m_corner3.height()-m_corner1.height()),
					  m_lfill,
					  m_lfill.rect());
	paint.drawPixmap (QRect (size().width()-m_rfill3.width(),
							 m_corner2.height(),
							 m_rfill3.width(),
							 size().height()-m_corner2.height()-m_corner3.height()),
					  m_rfill3,
					  m_rfill3.rect());

	int x = size().width();
	x -= m_rfill3.width();
	x -= m_rfill2.width();
	x -= m_rfill.width();

	paint.drawPixmap (QRect (x, m_corner2.height(),
							 m_rfill.width(),
							 size().height()-m_corner2.height()-m_corner3.height()),
					  m_rfill,
					  m_rfill.rect());

	paint.end ();

	m_scroller->move (size().width()-m_rfill3.width()-m_rfill2.width(),
					  m_corner2.height());
	m_scroller->resize (m_rfill2.width(),
						size().height()-m_corner2.height()-m_corner4.height());

}


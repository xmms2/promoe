
#include "Playlist.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QRect>
#include <QIcon>
#include <QApplication>
#include <QSettings>

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

void
dragButton::mouseMoveEvent (QMouseEvent *event)
{
	PlaylistWindow *pw = dynamic_cast<PlaylistWindow *>(window ());
	pw->resize (pw->size().width()+(event->pos().x()-m_diffx),
				pw->size().height()+(event->pos().y()-m_diffy));
}

PlaylistScroller::PlaylistScroller (PlaylistWidget *parent) : QWidget (parent)
{
	Skin *skin = Skin::getInstance ();

	m_pixmap = QPixmap(0,0);

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

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

PlaylistWindow::PlaylistWindow (QWidget *parent) : QMainWindow (parent)
{
	QSettings s;

#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif

	setWindowFlags (Qt::FramelessWindowHint);

	s.beginGroup ("playlist");
	if (!s.contains ("size")) {
		s.setValue ("size", QSize (275, 350));
	}
	resize (s.value("size").toSize ());

	m_playlist = new PlaylistWidget (this);
	setCentralWidget (m_playlist);
	m_shaded = new PlaylistShade (this);

	if (!s.contains ("shaded"))
		s.setValue ("shaded", false);
	else
		s.setValue ("shaded", !s.value("shaded").toBool ());
	
	switchDisplay ();

	s.endGroup ();

}

void
PlaylistWindow::switchDisplay (void)
{
	QSettings s;

	s.beginGroup ("playlist");

	if (!s.value("shaded").toBool ()) {
		m_shaded->hide ();

		m_playlist->show ();
		s.setValue ("shaded", true);
		m_playlist->resize (s.value("size").toSize ());
		resize (s.value("size").toSize ());

	} else {
		m_playlist->hide ();

		m_shaded->show ();
		s.setValue ("shaded", false);
		m_shaded->resize (s.value("size").toSize ().width(), 14);
		resize (s.value("size").toSize ().width(), 14);
	}

	s.endGroup ();

	update ();

}

void
PlaylistWindow::resizeEvent (QResizeEvent *event)
{
	QSettings s;

	if (s.value("playlist/shaded").toBool ()) {
		s.setValue ("playlist/size", size ());
	}
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
	move (event->globalPos().x() - m_diffx,
		  event->globalPos().y() - m_diffy);

}

void
PlaylistWindow::moveEvent (QMoveEvent *event)
{
	QSettings s;
	s.setValue ("playlist/pos", pos ());
}

void
PlaylistWindow::enterEvent (QEvent *event)
{
	m_playlist->setActive (true);
	m_shaded->setActive (true);
}


void
PlaylistWindow::leaveEvent (QEvent *event)
{
	m_playlist->setActive (false);
	m_shaded->setActive (false);
}



PlaylistWidget::PlaylistWidget (QWidget *parent) : QWidget (parent)
{
	Skin *skin = Skin::getInstance ();

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	setActive (underMouse ());

	m_view = new PlaylistView (this);
	m_view->move (10, 20);
	m_view->resize (size().width()-30, size().height()-20-38);

	m_list = new PlaylistList (m_view);

	m_scroller = new PlaylistScroller (this);
	connect (m_scroller, SIGNAL(scrolled(int)), this, SLOT(doScroll (int)));

	m_drag = new dragButton (this);

	setMinimumSize (275, 116);
	resize (275, 300);
}

void
PlaylistWidget::doScroll (int pos)
{
	int npos = ((float)pos) / (float)(m_scroller->getMax()) * float(m_list->height() - m_view->height());

	m_list->setOffset (npos);
	if (npos == 0) {
		m_list->update ();
	} else {
		m_list->scroll (0, npos);
	}
	QApplication::sendPostedEvents (m_list, 0);
}

void
PlaylistWidget::resizeEvent (QResizeEvent *event)
{
	m_view->resize (size().width()-30, size().height()-20-38);
	m_list->setSize (m_view->size().width(), m_view->size().height());
}

void
PlaylistWidget::setPixmaps (Skin *skin)
{
	setActive (m_active);
	resize (size().width(), size().height());
}

void
PlaylistWidget::setActive (bool active)
{
	Skin *skin = Skin::getInstance ();

	m_active = active;

	m_corner3 = skin->getPls (Skin::PLS_LCBAR);
	m_corner4 = skin->getPls (Skin::PLS_RCBAR);

	if (active) {
		m_corner1 = skin->getPls (Skin::PLS_CORNER_UL_0);
		m_corner2 = skin->getPls (Skin::PLS_CORNER_UR_0);
		m_titlebar = skin->getPls (Skin::PLS_TBAR_0);
		m_tfill = skin->getPls (Skin::PLS_TFILL_0);
		m_bfill = skin->getPls (Skin::PLS_BFILL_0);
		m_lfill = skin->getPls (Skin::PLS_LFILL_0);
		m_rfill = skin->getPls (Skin::PLS_RFILL_0);
		m_rfill2 = skin->getPls (Skin::PLS_RFILL2_0);
		m_rfill3 = skin->getPls (Skin::PLS_RFILL3_0);
	} else {
		m_corner1 = skin->getPls (Skin::PLS_CORNER_UL_1);
		m_corner2 = skin->getPls (Skin::PLS_CORNER_UR_1);
		m_titlebar = skin->getPls (Skin::PLS_TBAR_1);
		m_tfill = skin->getPls (Skin::PLS_TFILL_1);
		m_bfill = skin->getPls (Skin::PLS_BFILL_1);
		m_lfill = skin->getPls (Skin::PLS_LFILL_1);
		m_rfill = skin->getPls (Skin::PLS_RFILL_1);
		m_rfill2 = skin->getPls (Skin::PLS_RFILL2_1);
		m_rfill3 = skin->getPls (Skin::PLS_RFILL3_1);
	}

	update ();
}

void
PlaylistWidget::mouseDoubleClickEvent (QMouseEvent *event)
{
	PlaylistWindow *pw = dynamic_cast<PlaylistWindow *>(window ());
	if (event->pos().y() < 14) {
		pw->switchDisplay ();
	}

}

void
PlaylistWidget::paintEvent (QPaintEvent *event)
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

	m_drag->move (size().width()-30,
				  size().height()-30);
	m_drag->resize (30, 30);

}


#include "MainWindow.h"
#include "Playlist.h"
#include "PlaylistList.h"

#include "PlaylistShade.h"
#include "PlaylistMenu.h"
#include "FileDialog.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QRect>
#include <QIcon>
#include <QApplication>
#include <QSettings>
#include <QFileDialog>

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

	m_slider->doScroll ();
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
		
void
PlaylistScroller::setMax (uint max) 
{ 
	m_max = max;
	repositionButton ();
}

uint
PlaylistScroller::getMax (void)
{
	return rect().height()-m_button->rect().height();
}

uint
PlaylistScroller::getPos (void)
{
	return (int)((float)(m_button->pos ().y ()) / (float)getMax() * (float)m_max);
}

void
PlaylistScroller::setPixmaps (Skin *skin)
{
	m_pixmap = skin->getPls (Skin::PLS_RFILL2_0);
}

void
PlaylistScroller::repositionButton (void)
{
	PlaylistWidget *pw = dynamic_cast<PlaylistWidget *>(parent ());
	/*  x = 182.6 / (454 - 242) * 224 */
	int bpos = (int)((float)(pw->getOffset ()) / (float)m_max * (float) getMax ());
	if (bpos > getMax ()) 
		bpos = getMax ();
	m_button->move (0, bpos);
}

void 
PlaylistScroller::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull ()) {
		return;
	}

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (event->rect (), m_pixmap, m_pixmap.rect ());
	paint.end ();
}

PlaylistWindow::PlaylistWindow (QWidget *parent) : QMainWindow (parent)
{
	QSettings s;
	m_mw = dynamic_cast<MainWindow *>(parent);
#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif

	setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);
	setAttribute (Qt::WA_DeleteOnClose);

	s.beginGroup ("playlist");
	if (!s.contains ("size")) {
		s.setValue ("size", QSize (275, 350));
	}
	resize (s.value("size").toSize ());

	m_playlist = new PlaylistWidget (this);
	setCentralWidget (m_playlist);
	m_shaded = new PlaylistShade (this);

	m_shadebtn = new Button (this, Skin::PLS_SHADE_BTN_0, Skin::PLS_SHADE_BTN_1, true);
	connect (m_shadebtn, SIGNAL (clicked()), this, SLOT (switchDisplay ()));
	m_shadebtn->move(size().width() - 21, 3);

	m_closebtn = new Button (this, Skin::PLS_CLOSE_BTN_0, Skin::PLS_CLOSE_BTN_1, true);
	connect (m_closebtn, SIGNAL (clicked()), this, SLOT (togglePL ()));
	m_closebtn->move(size().width() - 11, 3);

	if (!s.contains ("shaded"))
		s.setValue ("shaded", false);
	else
		s.setValue ("shaded", !s.value("shaded").toBool ());
	
	switchDisplay ();

	s.endGroup ();

}

void 
PlaylistWindow::togglePL (void)
{
	m_mw->togglePL(true);
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

	m_shadebtn->move(size().width() - 21, 3);
	m_closebtn->move(size().width() - 11, 3);

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
	connect (m_list, SIGNAL(sizeChanged(QSize)), this, SLOT(sizeChangedList (QSize)));

	m_drag = new dragButton (this);
	m_drag->resize (30, 30);
	
	addButtons ();

	setMinimumSize (275, 116);
	resize (275, 300);
}

void 
PlaylistWidget::addButtons (void)
{
	PlaylistMenuButton *b;

	m_add = new PlaylistMenu (this, Skin::PLS_ADD,
							  Skin::PLS_ADD_DEC);
	b = new PlaylistMenuButton (m_add, Skin::PLS_ADD_URL_0,
								Skin::PLS_ADD_URL_1);
	connect (b, SIGNAL(activated ()), this, SLOT (menuAddUrl ()));
	b = new PlaylistMenuButton (m_add, Skin::PLS_ADD_DIR_0,
								Skin::PLS_ADD_DIR_1);
	connect (b, SIGNAL(activated ()), this, SLOT (menuAddDir ()));
	b = new PlaylistMenuButton (m_add, Skin::PLS_ADD_FIL_0,
								Skin::PLS_ADD_FIL_1);
	connect (b, SIGNAL(activated ()), this, SLOT (menuAddFile ()));


	m_del = new PlaylistMenu (this, Skin::PLS_DEL,
							  Skin::PLS_DEL_DEC);
	b = new PlaylistMenuButton (m_del, Skin::PLS_MSC_BTN_0,
								Skin::PLS_MSC_BTN_1);
	b = new PlaylistMenuButton (m_del, Skin::PLS_DEL_ALL_0,
								Skin::PLS_DEL_ALL_1);
	connect (b, SIGNAL(activated ()),
	         &XMMSHandler::getInstance(), SLOT (playlistClear ()));
	b = new PlaylistMenuButton (m_del, Skin::PLS_DEL_CRP_0,
								Skin::PLS_DEL_CRP_1);
	b = new PlaylistMenuButton (m_del, Skin::PLS_DEL_FIL_0,
								Skin::PLS_DEL_FIL_1);
	connect (b, SIGNAL(activated ()), m_list, SLOT (deleteFiles ()));

	m_sel = new PlaylistMenu (this, Skin::PLS_SEL,
							  Skin::PLS_SEL_DEC);
	b = new PlaylistMenuButton (m_sel, Skin::PLS_SEL_INV_0,
								Skin::PLS_SEL_INV_1);
	b = new PlaylistMenuButton (m_sel, Skin::PLS_SEL_NIL_0,
								Skin::PLS_SEL_NIL_1);
	b = new PlaylistMenuButton (m_sel, Skin::PLS_SEL_ALL_0,
								Skin::PLS_SEL_ALL_1);

	m_msc = new PlaylistMenu (this, Skin::PLS_MSC,
							  Skin::PLS_MSC_DEC);
	b = new PlaylistMenuButton (m_msc, Skin::PLS_MSC_SRT_0,
								Skin::PLS_MSC_SRT_1);
	b = new PlaylistMenuButton (m_msc, Skin::PLS_MSC_INF_0,
								Skin::PLS_MSC_INF_1);
	b = new PlaylistMenuButton (m_msc, Skin::PLS_MSC_OPT_0,
								Skin::PLS_MSC_OPT_1);

	m_lst = new PlaylistMenu (this, Skin::PLS_LST,
							  Skin::PLS_LST_DEC);
	b = new PlaylistMenuButton (m_lst, Skin::PLS_LST_NEW_0,
								Skin::PLS_LST_NEW_1);
	b = new PlaylistMenuButton (m_lst, Skin::PLS_LST_SAV_0,
								Skin::PLS_LST_SAV_1);
	b = new PlaylistMenuButton (m_lst, Skin::PLS_LST_OPN_0,
								Skin::PLS_LST_OPN_1);
}

void
PlaylistWidget::diveDir (const QString &dir)
{
	QDir d (dir);

	d.setFilter (QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

	QFileInfoList list = d.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.isDir ()) {
			diveDir (fileInfo.filePath ());
		} else {
			QString fname = fileInfo.filePath();
			XMMSHandler::getInstance ().playlistAddURL ("file://" + fname);
		}
	}
}

void
PlaylistWidget::menuAddDir ()
{
	/*
	QString dir;
	dir = QFileDialog::getExistingDirectory (this, "Select files to play",
											 QDir::homePath ());
	diveDir (dir);
	*/

	FileDialog fd (this, "playlist_add_dir");
	QString dir = fd.getDirectory ();
	if (!dir.isNull ())
		diveDir (dir);
}

void
PlaylistWidget::menuAddFile ()
{
	QStringList files;
	FileDialog fd (this, "playlist_add_files");

	files = fd.getFiles ();

	for (int i = 0; i < files.count(); i++) {
		XMMSHandler::getInstance ().playlistAddURL ("file://" + files.value(i));
	}

}

void
PlaylistWidget::sizeChangedList (QSize s)
{
	m_scroller->setMax (s.height() - m_view->height());
}

void
PlaylistWidget::doScroll (int pos)
{
	m_list->setOffset (pos);
	if (pos == 0) {
		m_list->update ();
	} else {
		m_list->scroll (0, pos);
	}
	QApplication::sendPostedEvents (m_list, 0);
}

void
PlaylistWidget::resizeEvent (QResizeEvent *event)
{
	m_view->resize (size().width()-30, size().height()-20-38);
	m_list->setSize (m_view->size().width(), m_view->size().height());

	/* since the sizes has changed we need to move the buttons */
	m_scroller->move (size().width()-m_rfill3.width()-m_rfill2.width(),
					  m_corner2.height());
	m_scroller->resize (m_rfill2.width(),
						size().height()-m_corner2.height()-m_corner4.height());

	m_scroller->setMax (m_list->height() - m_view->height());

	/* drag corner */
	m_drag->move (size().width()-30,
				  size().height()-30);

	/* move add menu */
	m_add->move (11, height() - m_add->height() - 12);
	m_del->move (40, height() - m_del->height() - 12);
	m_sel->move (69, height() - m_sel->height() - 12);
	m_msc->move (98, height() - m_msc->height() - 12);
	m_lst->move (width()-22-25, height() - m_lst->height() - 12);
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


uint 
PlaylistWidget::getOffset (void)
{ 
	return m_list->getOffset (); 
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
	QRect r;

	paint.begin (this);
	
	/* drawing the upper left corner */
	r.setRect (0, 0, 
			   m_corner1.width(),
			   m_corner1.height());
	paint.drawPixmap (r, m_corner1, m_corner1.rect ());

	/* drawing the upper right corner */
	r.setRect (width()-m_corner2.width(), 0, m_corner2.width(), m_corner2.height());
	paint.drawPixmap (r, m_corner2, m_corner2.rect());

	/* Drawing the lower left corner */
	r.setRect (0, size().height()-m_corner3.height(), 
			   m_corner3.width(), m_corner3.height());
	paint.drawPixmap (r, m_corner3, m_corner3.rect());

	/* drawing the lower right corner */
	r.setRect (size().width()-m_corner4.width(),
			   size().height()-m_corner4.height(),
			   m_corner4.width(),
			   m_corner4.height());
	paint.drawPixmap (r, m_corner4, m_corner4.rect());

	/* calculate middle of the bar */
	int midx = (size().width()/2) - (m_titlebar.width()/2);

	/* Add the titlebar */
	r.setRect (midx, 0, m_titlebar.width(), m_titlebar.height());
	paint.drawPixmap (r, m_titlebar, m_titlebar.rect());

	/* left fill */
	r.setRect (m_corner1.width(), 0, midx - m_corner1.width(), m_tfill.height());
	paint.drawPixmap (r, m_tfill, m_tfill.rect());

	/* Calculate middle pixel to the right side of the titlebar */
	int midx2 = midx + m_titlebar.width();

	/* right fill */
	r.setRect (midx2, 0, width()-midx2-m_corner2.width(), m_tfill.height());
	paint.drawPixmap (r, m_tfill, m_tfill.rect());

	/* calculate the size of the bottom side */
	int bsize = m_corner3.width()+m_corner4.width();

	/* if the width is bigger than bottom size we need to pad with
	 * a bit of generic bottom */
	if (size().width() > bsize) {
		/* calculate padding width */
		int pad_to = size().width() - bsize;

		/* Draw the bottom filling */
		r.setRect (m_corner3.width(), size().height()-m_bfill.height(), 
				   pad_to, m_bfill.height());
		paint.drawPixmap (r, m_bfill, m_bfill.rect());
	}

	/* The slider bar consists of three elements L|S|R (left, slider, right)
	 * here we paint L and R and let the slider take care of the rest.
	 */
	r.setRect (0, m_corner1.height(), m_lfill.width(), 
			   size().height()-m_corner3.height()-m_corner1.height());
	paint.drawPixmap (r,m_lfill, m_lfill.rect());

	r.setRect (size().width()-m_rfill3.width(), m_corner2.height(), m_rfill3.width(),
			   size().height()-m_corner2.height()-m_corner3.height());
	paint.drawPixmap (r, m_rfill3, m_rfill3.rect());

	/* figure out where to place the last padding */
	int x = size().width();
	x -= m_rfill3.width(); /* minus R */
	x -= m_rfill2.width(); /* minus slider */
	x -= m_rfill.width(); /* minus L */

	r.setRect (x, m_corner2.height(), m_rfill.width(), 
			   size().height()-m_corner2.height()-m_corner3.height());
	paint.drawPixmap (r, m_rfill, m_rfill.rect());

	paint.end ();
}


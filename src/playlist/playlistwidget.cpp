/**
 *  This file is a part of Prome, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
 *  Copyright (C) 2007 Thomas Frauendorfer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "XMMSHandler.h"

#include "mainwindow.h"
#include "BrowseDialog.h"
#include "playlistwindow.h"
#include "playlistwidget.h"
#include "playlistview.h"
#include "playlistmodel.h"

#include "playlistshade.h"
#include "playlistmenu.h"
#include "FileDialog.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QRect>
#include <QIcon>
#include <QApplication>
#include <QSettings>
#include <QFileDialog>

/*
 *
 * PlaylistScrollBar
 *
 */
PlaylistScrollBar::PlaylistScrollBar (QWidget *parent) :
                   QScrollBar (Qt::Vertical, parent)
{
	Skin *skin = Skin::getInstance ();

	setContextMenuPolicy(Qt::NoContextMenu);

	m_pixmap = QPixmap (0, 0);
	m_slider = QPixmap (0, 0);
	m_slider_down = QPixmap (0, 0);

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));
}

void
PlaylistScrollBar::mouseMoveEvent (QMouseEvent *event)
{
	if (!isSliderDown ())
		return;

	int tmp = sliderValueFromPosition(event->y () - m_sliderOffset);
	if (tmp == value())
		return;

	setValue(tmp);

	//TODO only repaint necessary range
	repaint ();
}

void
PlaylistScrollBar::mousePressEvent (QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->ignore();
		return;
	}

	if (maximum () == minimum ())
		return;

	int sliderBase = sliderPositionFromValue();
	if (event->y () < sliderBase) {
		triggerAction (QAbstractSlider::SliderPageStepSub);
	} else if (event->y () > sliderBase + m_slider.height ()) {
		triggerAction (QAbstractSlider::SliderPageStepAdd);
	} else {
		m_sliderOffset = event->y () - sliderBase;
		setSliderDown (true);
	}

	//TODO only repaint necessary range
	repaint ();
}

void
PlaylistScrollBar::mouseReleaseEvent (QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		event->ignore();
		return;
	}

	if (isSliderDown ()) {
		setValue(sliderValueFromPosition(event->y () - m_sliderOffset));
		setSliderDown (false);
	}

	//TODO only repaint necessary range
	repaint ();
}

void
PlaylistScrollBar::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull ()) {
		return;
	}

// TODO remove, in here for debuging
//	qDebug("%i %i %i %i %i %i %i", event->rect ().x(), event->rect ().y(), event->rect ().width(), event->rect ().height(), 
//	       minimum(), maximum (), sliderPosition ());

	QPainter (paint);
	paint.begin (this);
	/* draw background */
	paint.drawPixmap (event->rect (), m_pixmap, m_pixmap.rect ());
	/* draw slider */
	QPixmap *slider = isSliderDown () ? &m_slider_down : &m_slider ;
	QRect rect (slider->rect ());
	rect.moveTop (sliderPositionFromValue ());
	paint.drawPixmap (rect , *slider, slider->rect ());
	paint.end ();
}


void
PlaylistScrollBar::setPixmaps (Skin *skin)
{
	m_pixmap = skin->getPls (Skin::PLS_RFILL2_0);
	m_slider = skin->getPls (Skin::PLS_SCROLL_0);
	m_slider_down = skin->getPls (Skin::PLS_SCROLL_1);
}


int
PlaylistScrollBar::sliderPositionFromValue ()
{
	return QStyle::sliderPositionFromValue (minimum (), maximum (),
	                                        sliderPosition (),
	                                        height () - m_slider.height (),
	                                        false);
}

int
PlaylistScrollBar::sliderValueFromPosition (int position)
{
	return  QStyle::sliderValueFromPosition (minimum (), maximum (),
	                                         position,
	                                         height () - m_slider.height (),
                                             false);
}



/*
 * PlaylistWidget
 */
PlaylistWidget::PlaylistWidget (QWidget *parent) : QWidget (parent)
{
	Skin *skin = Skin::getInstance ();

	connect (skin, SIGNAL (skinChanged (Skin *)),
	         this, SLOT (setPixmaps(Skin *)));

	setActive (underMouse ());

	m_view = new PlaylistView (this);
	m_view->move (10, 20);
	m_view->resize (size().width()-30, size().height()-20-38);
	// TODO: creation of Playlistmodel should be done elsewhere
	m_view->setModel (XMMSHandler::getInstance().getPlaylistModel());

	//m_list = new PlaylistList (m_view);

	/*
	 * This is a hack to make PlaylistScrollBar work with PlaylistView.
	 * It is necessery because of limitations and at least one Bug in the
	 *  QT library (as of Version 4.3)
	 * TODO: This might break in a future Qt version. Try to find a better solution
	 */
	m_scrollBar = new PlaylistScrollBar (this);
	m_view->setVerticalScrollBar (m_scrollBar);
	m_scrollBar->setParent(this);
	m_scrollBar->show();
	/* Workarounds for another QT bug (at least in my opinion) */
	connect (m_scrollBar, SIGNAL(actionTriggered (int)),
			 m_view, SLOT(verticalScrollbarAction (int)));
	connect (m_scrollBar, SIGNAL(valueChanged (int)),
	         m_view, SLOT(verticalScrollbarValueChanged (int)));

	m_sizegrip = new PlaylistSizeGrip(this);
	m_sizegrip->resize (20, 20);

	addButtons ();

	setMinimumSize (275, 116);
	resize (275, 300);
}

void
PlaylistWidget::addButtons (void)
{
	PlaylistMenuButton *b;

	/* Add menu */
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

	/* Del menu */
	m_del = new PlaylistMenu (this, Skin::PLS_DEL,
							  Skin::PLS_DEL_DEC);
	b = new PlaylistMenuButton (m_del, Skin::PLS_MSC_BTN_0,
								Skin::PLS_MSC_BTN_1);
	b = new PlaylistMenuButton (m_del, Skin::PLS_DEL_ALL_0,
								Skin::PLS_DEL_ALL_1);
	connect (b, SIGNAL (activated ()),
	         &XMMSHandler::getInstance(), SLOT (playlistClear ()));
	b = new PlaylistMenuButton (m_del, Skin::PLS_DEL_CRP_0,
								Skin::PLS_DEL_CRP_1);
	connect (b, SIGNAL (activated ()),
	         m_view, SLOT (cropSelected  ()));
	b = new PlaylistMenuButton (m_del, Skin::PLS_DEL_FIL_0,
								Skin::PLS_DEL_FIL_1);
	connect (b, SIGNAL (activated ()),
	         m_view, SLOT (removeSelected ()));
//	connect (b, SIGNAL(activated ()), m_list, SLOT (deleteFiles ()));

	/* Selection menu */
	m_sel = new PlaylistMenu (this, Skin::PLS_SEL,
							  Skin::PLS_SEL_DEC);
	b = new PlaylistMenuButton (m_sel, Skin::PLS_SEL_INV_0,
								Skin::PLS_SEL_INV_1);
	connect (b, SIGNAL (activated ()),
	         m_view, SLOT (invertSelection ()));
	b = new PlaylistMenuButton (m_sel, Skin::PLS_SEL_NIL_0,
								Skin::PLS_SEL_NIL_1);
	connect (b, SIGNAL (activated ()),
	         m_view, SLOT (clearSelection ()));
	b = new PlaylistMenuButton (m_sel, Skin::PLS_SEL_ALL_0,
								Skin::PLS_SEL_ALL_1);
	connect (b, SIGNAL (activated ()),
	         m_view, SLOT (selectAll ()));

	/* misc menu */
	m_msc = new PlaylistMenu (this, Skin::PLS_MSC,
							  Skin::PLS_MSC_DEC);
	b = new PlaylistMenuButton (m_msc, Skin::PLS_MSC_SRT_0,
								Skin::PLS_MSC_SRT_1);
	b = new PlaylistMenuButton (m_msc, Skin::PLS_MSC_INF_0,
								Skin::PLS_MSC_INF_1);
	b = new PlaylistMenuButton (m_msc, Skin::PLS_MSC_OPT_0,
								Skin::PLS_MSC_OPT_1);
	/* playlist menu */
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
	QSettings s;
	QStringList files;

	if (s.value ("playlist/useremote").toBool () == true) {
		BrowseDialog bd (window ());
		files = bd.getFiles ();
		for (int i = 0; i < files.count(); i++) {
			XMMSHandler::getInstance ().playlistAddURL (files.value (i));
		}
	} else {
		FileDialog fd (this, "playlist_add_files");

		files = fd.getFiles ();

		for (int i = 0; i < files.count(); i++) {
			XMMSHandler::getInstance ().playlistAddURL ("file://" + files.value(i));
		}
	}

}

void
PlaylistWidget::resizeEvent (QResizeEvent *event)
{
	m_view->resize (size().width()-30, size().height()-20-38);

	/* since the sizes has changed we need to move the buttons */
	m_scrollBar->move (size().width()-m_rfill3.width()-m_rfill2.width(),
					  m_corner2.height());
	m_scrollBar->resize (m_rfill2.width(),
						size().height()-m_corner2.height()-m_corner4.height());

	/* place the sizegrip in the lower right corner */
	m_sizegrip->move( size().width() - m_sizegrip->width(),
	                  size().height() - m_sizegrip->height() );

	/* move menus */
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


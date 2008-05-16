/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
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

#include <iostream>

#include "Skin.h"

#include <QDir>
#include <QPainter>

Skin *Skin::singleton = NULL;

Skin *Skin::getInstance (void)
{
	if (!singleton) {
		singleton = new Skin ();
	}

	return singleton;
}

Skin::Skin ()
{
	setSizes ();
	setPositions ();
}

void
Skin::setSizes ()
{
	m_sizes.clear ();
	m_sizes << QSize ( 9,  9) // BUTTON_MW_MENU
	        << QSize ( 9,  9) // BUTTON_MW_MINIMIZE
	        << QSize ( 9,  9) // BUTTON_MW_CLOSE
	        << QSize ( 9,  9) // BUTTON_MW_SHADE
	        << QSize (23, 18) // BUTTON_MW_PREV
	        << QSize (23, 18) // BUTTON_MW_PLAY
	        << QSize (23, 18) // BUTTON_MW_PAUSE
	        << QSize (23, 18) // BUTTON_MW_STOP
	        << QSize (22, 18) // BUTTON_MW_NEXT
	        << QSize (22, 16) // BUTTON_MW_EJECT
	        << QSize (23, 12) // BUTTON_MW_EQ
	        << QSize (23, 12) // BUTTON_MW_PLS
	        << QSize (46, 15) // BUTTON_MW_SHUFFLE
	        << QSize (28, 15) // BUTTON_MW_REPEAT
	        << QSize ( 9,  9) // BUTTON_MW_SHADED_UNSHADE
	        << QSize ( 9,  9) // BUTTON_EQ_CLOSE
	        << QSize ( 9,  9) // BUTTON_EQ_SHADE
	        << QSize (25, 12) // BUTTON_EQ_ACTIVE
	        << QSize (33, 12) // BUTTON_EQ_AUTO
	        << QSize (44, 12) // BUTTON_EQ_PRESET
	        << QSize () // BUTTON_EQ_SHADED_CLOSE
	        << QSize () // BUTTON_EQ_SHADED_UNSHADE
	        << QSize ( 9,  9) // BUTTON_PLS_CLOSE
	        << QSize ( 9,  9) // BUTTON_PLS_SHADE
	        << QSize ( 9,  9) // BUTTON_PLS_SHADED_UNSHADE
	        << QSize () // SLIDER_POSBAR
	        << QSize (248, 10) // SLIDER_POSBAR_BGS
	        << QSize (14, 11) // SLIDER_VOLUMEBAR
	        << QSize (68, 13) // SLIDER_VOLUMEBAR_BGS
	        << QSize () // SLIDER_BALANCEBAR
	        << QSize (38, 13) // SLIDER_BALANCEBAR_BGS
	        << QSize () // SLIDER_EQUALIZER
	        << QSize (14, 63) // SLIDER_EQUALIZER_BGS
	        ;
}

void
Skin::setPositions ()
{
	m_positions.clear ();
	m_positions << QPoint (  6,  3) // BUTTON_MW_MENU
	            << QPoint (244,  3) // BUTTON_MW_MINIMIZE
	            << QPoint (264,  3) // BUTTON_MW_CLOSE
	            << QPoint (254,  3) // BUTTON_MW_SHADE
	            << QPoint ( 16, 88) // BUTTON_MW_PREV
	            << QPoint ( 39, 88) // BUTTON_MW_PLAY
	            << QPoint ( 62, 88) // BUTTON_MW_PAUSE
	            << QPoint ( 85, 88) // BUTTON_MW_STOP
	            << QPoint (108, 88) // BUTTON_MW_NEXT
	            << QPoint (136, 89) // BUTTON_MW_EJECT
	            << QPoint (219, 58) // BUTTON_MW_EQ
	            << QPoint (242, 58) // BUTTON_MW_PLS
	            << QPoint (164, 89) // BUTTON_MW_SHUFFLE
	            << QPoint (210, 89) // BUTTON_MW_REPEAT
	            << QPoint (254,  3) // BUTTON_MW_SHADED_UNSHADE
	            << QPoint (264,  3) // BUTTON_EQ_CLOSE
	            << QPoint (254  ,3) // BUTTON_EQ_SHADE
	            << QPoint ( 14, 18) // BUTTON_EQ_ACTIVE
	            << QPoint ( 39, 18) // BUTTON_EQ_AUTO
	            << QPoint (217, 18) // BUTTON_EQ_PRESET
	            << QPoint () // BUTTON_EQ_SHADED_CLOSE
	            << QPoint () // BUTTON_EQ_SHADED_UNSHADE
	            // The next 3 widgets are placed from the right
	            << QPoint (-11,  3) // BUTTON_PLS_CLOSE
	            << QPoint (-20,  3) // BUTTON_PLS_SHADE
	            << QPoint (-20,  3) // BUTTON_PLS_SHADED_UNSHADE
	            << QPoint () // SLIDER_POSBAR
	            << QPoint ( 16, 72) // SLIDER_POSBAR_BGS
	            << QPoint () // SLIDER_VOLUMEBAR
	            << QPoint (107, 57) // SLIDER_VOLUMEBAR_BGS
	            << QPoint () // SLIDER_BALANCEBAR
	            << QPoint (177, 57) // SLIDER_BALANCEBAR_BGS
	            << QPoint () // SLIDER_EQUALIZER
	            << QPoint () // SLIDER_EQUALITER_BGS
	            ;
}

void
Skin::BuildEqualizer (void)
{
	QPixmap *img = getPixmap ("eqmain");
	QPixmap *imgex = getPixmap ("eq_ex");
	if (img && imgex) {
		m_items[EQ_WIN_BG] = img->copy (0, 0, 275, 116);

		if (img->height () > 294) {
			m_items[EQ_WIN_GRAPH_BG] = img->copy (0, 294, 113, qMin (19, img->height () - 294));
		} else {
			m_items[EQ_WIN_GRAPH_BG] = QPixmap ();
		}

		QIcon icon;
		icon.addPixmap (img->copy (  0, 116,  9,  9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (  0, 125,  9,  9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_EQ_CLOSE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (254,  3,  9,  9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (imgex->copy ( 1, 38,  9,  9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_EQ_SHADE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy ( 10, 119, 25, 12), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (128, 119, 25, 12), QIcon::Active, QIcon::Off);
		icon.addPixmap (img->copy ( 69, 119, 25, 12), QIcon::Normal, QIcon::On);
		icon.addPixmap (img->copy (187, 119, 25, 12), QIcon::Active, QIcon::On);
		m_icons[BUTTON_EQ_ACTIVE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy ( 35, 119, 33, 12), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (153, 119, 33, 12), QIcon::Active, QIcon::Off);
		icon.addPixmap (img->copy ( 94, 119, 33, 12), QIcon::Normal, QIcon::On);
		icon.addPixmap (img->copy (212, 119, 33, 12), QIcon::Active, QIcon::On);
		m_icons[BUTTON_EQ_AUTO] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (224, 164, 44, 12), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (224, 176, 44, 12), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_EQ_PRESET] = icon;

		QPixmapList list;
		for (int i = 0; i < 14; i++) {
			list << img->copy (13+15*i, 164, 14, 63);
		}
		for (int i = 0; i < 14; i++) {
			list << img->copy (13+15*i, 229, 14, 63);
		}
		m_backgrounds[SLIDER_EQUALIZER_BGS] = list;

		m_items[EQ_WIN_BAR_BTN_0] = img->copy (0, 164, 11, 11);
		m_items[EQ_WIN_BAR_BTN_1] = img->copy (0, 176, 11, 11);

		delete img;
	} else {
		setSkin(":CleanAMP/");
	}
}

void
Skin::BuildPlaylist (void)
{
	QPixmap tmp;

	QPixmap *img = getPixmap ("pledit");

	if(img)
	{
		QIcon icon;
		icon.addPixmap (img->copy (167,  3,  9,  9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy ( 52, 42,  9,  9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_PLS_CLOSE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (158,  3,  9,  9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy ( 62, 42,  9,  9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_PLS_SHADE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (129, 45,  9,  9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (150, 42,  9,  9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_PLS_SHADED_UNSHADE] = icon;

		m_playlist[PLS_CORNER_UL_0] = img->copy(0, 0, 25, 20);
		m_playlist[PLS_CORNER_UL_1] = img->copy(0, 21, 25, 20);

		m_playlist[PLS_TBAR_0] = img->copy (26, 0, 100, 20);
		m_playlist[PLS_TBAR_1] = img->copy (26, 21, 100, 20);

		m_playlist[PLS_CORNER_UR_0] = img->copy(153, 0, 25, 20);
		m_playlist[PLS_CORNER_UR_1] = img->copy(153, 21, 25, 20);

		m_playlist[PLS_TFILL_0] = img->copy(127, 0, 25, 20);
		m_playlist[PLS_TFILL_1] = img->copy(127, 21, 25, 20);

		m_playlist[PLS_BFILL_0] = img->copy(179, 0, 25, 38);

		m_playlist[PLS_VISMINI_0] = img->copy(205, 0, 75, 38);

		m_playlist[PLS_LFILL_0] = img->copy(0, 42, 12, 29);

		m_playlist[PLS_RFILL_0] = img->copy(31, 42, 5, 29);
		m_playlist[PLS_RFILL2_0] = img->copy(36, 42, 8, 29);
		m_playlist[PLS_RFILL3_0] = img->copy(44, 42, 7, 29);

		tmp = m_playlist[PLS_CORNER_UR_0];

//		m_playlist[PLS_CLOSE_BTN_0] = tmp.copy(14, 3, 9, 9);
//		m_playlist[PLS_CLOSE_BTN_1] = img->copy(52, 42, 9, 9);

//		m_playlist[PLS_SHADE_BTN_0] = tmp.copy(5, 3, 9, 9);
//		m_playlist[PLS_SHADE_BTN_1] = img->copy(62, 42, 9, 9);

		m_playlist[PLS_MAX_BTN_0] = img->copy(150, 42, 9, 9);

		m_playlist[PLS_SCROLL_0] = img->copy(52, 53, 8, 18);
		m_playlist[PLS_SCROLL_1] = img->copy(61, 53, 8, 18);

		m_playlist[PLS_WS_LE_0] = img->copy(72, 42, 25, 14);
		m_playlist[PLS_WS_RE_0] = img->copy(99, 42, 50, 14);
		m_playlist[PLS_WS_RE_1] = img->copy(99, 57, 50, 14);
		m_playlist[PLS_WS_MID_0] = img->copy(72, 57, 25, 14);

		m_playlist[PLS_LCBAR] = img->copy(0, 72, 125, 38);
		m_playlist[PLS_RCBAR] = img->copy(126, 72, 150, 38);

		/* extract the buttons */
		m_playlist[PLS_ADD] = img->copy(11, 80, 25, 18);
		m_playlist[PLS_ADD_DEC] = img->copy(48, 111, 3, 54);
		m_playlist[PLS_ADD_URL_0] = img->copy(0, 111, 22, 18);
		m_playlist[PLS_ADD_URL_1] = img->copy(23, 111, 22, 18);

		m_playlist[PLS_ADD_DIR_0] = img->copy(0, 130, 22, 18);
		m_playlist[PLS_ADD_DIR_1] = img->copy(23, 130, 22, 18);

		m_playlist[PLS_ADD_FIL_0] = img->copy(0, 149, 22, 18);
		m_playlist[PLS_ADD_FIL_1] = img->copy(23, 149, 22, 18);

		/* Delete buttons */
		m_playlist[PLS_DEL] = img->copy(40, 80, 25, 18);
		m_playlist[PLS_DEL_DEC] = img->copy(100, 111, 3, 72);
		m_playlist[PLS_DEL_ALL_0] = img->copy(54, 111, 22, 18);
		m_playlist[PLS_DEL_ALL_1] = img->copy(77, 111, 22, 18);

		m_playlist[PLS_DEL_CRP_0] = img->copy(54, 130, 22, 18);
		m_playlist[PLS_DEL_CRP_1] = img->copy(77, 130, 22, 18);

		m_playlist[PLS_DEL_FIL_0] = img->copy(54, 149, 22, 18);
		m_playlist[PLS_DEL_FIL_1] = img->copy(77, 149, 22, 18);

		/* Select buttons */
		m_playlist[PLS_SEL] = img->copy(69, 80, 25, 18);
		m_playlist[PLS_SEL_DEC] = img->copy(150, 111, 3, 54);
		m_playlist[PLS_SEL_INV_0] = img->copy(104, 111, 22, 18);
		m_playlist[PLS_SEL_INV_1] = img->copy(127, 111, 22, 18);

		m_playlist[PLS_SEL_NIL_0] = img->copy(104, 130, 22, 18);
		m_playlist[PLS_SEL_NIL_1] = img->copy(127, 130, 22, 18);

		m_playlist[PLS_SEL_ALL_0] = img->copy(104, 149, 22, 18);
		m_playlist[PLS_SEL_ALL_1] = img->copy(127, 149, 22, 18);

		/* misc buttons */
		m_playlist[PLS_MSC] = img->copy(98, 80, 25, 18);
		m_playlist[PLS_MSC_DEC] = img->copy(200, 111, 3, 54);
		m_playlist[PLS_MSC_SRT_0] = img->copy(154, 111, 22, 18);
		m_playlist[PLS_MSC_SRT_1] = img->copy(177, 111, 22, 18);

		m_playlist[PLS_MSC_INF_0] = img->copy(154, 130, 22, 18);
		m_playlist[PLS_MSC_INF_1] = img->copy(177, 130, 22, 18);

		m_playlist[PLS_MSC_OPT_0] = img->copy(154, 149, 22, 18);
		m_playlist[PLS_MSC_OPT_1] = img->copy(177, 149, 22, 18);

		/* list buttons */
		m_playlist[PLS_LST] = img->copy(229, 80, 25, 18);
		m_playlist[PLS_LST_DEC] = img->copy(250, 111, 3, 54);
		m_playlist[PLS_LST_NEW_0] = img->copy(204, 111, 22, 18);
		m_playlist[PLS_LST_NEW_1] = img->copy(227, 111, 22, 18);

		m_playlist[PLS_LST_SAV_0] = img->copy(204, 130, 22, 18);
		m_playlist[PLS_LST_SAV_1] = img->copy(227, 130, 22, 18);

		m_playlist[PLS_LST_OPN_0] = img->copy(204, 149, 22, 18);
		m_playlist[PLS_LST_OPN_1] = img->copy(227, 149, 22, 18);

		/* misc button */
		m_playlist[PLS_MSC_BTN_0] = img->copy(54, 168, 22, 18);
		m_playlist[PLS_MSC_BTN_1] = img->copy(77, 168, 22, 18);

		delete img;
	}
	else
		setSkin(":CleanAMP/");

}

void
Skin::setSkin (const QString& name)
{
	m_path = name;

	m_items.clear();
	BuildButtons();
	BuildToggleButtons();
	BuildTitleBar();
	BuildOther();
	BuildEqualizer ();

	m_letterMap.clear();
	BuildLetterMap();

	BuildSliders();

	m_pledit_txt.clear();
	ParsePLEdit();

	m_numbers.clear();
	BuildNumbers();

	m_playlist.clear ();
	BuildPlaylist ();

	emit skinChanged(this);
}

QPixmap
Skin::getPixmap (const QString& file, QDir dir)
{
	/* check for files in zip and check if file exists */

	dir.setFilter (QDir::Files);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QString fname = fileInfo.fileName().toLower();
		if (fname.section(".", 0, 0) == file) {
			return QPixmap (fileInfo.filePath());
		}
	}

	return QPixmap (0,0);
}


QPixmap *
Skin::getPixmap (const QString& file)
{
	QDir dir;

	dir.setPath (m_path);
	dir.setFilter (QDir::Files);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QString fname = fileInfo.fileName().toLower();
		if (fname.section(".", 0, 0) == file) {
			return new QPixmap (fileInfo.filePath());
		}
	}

	return NULL;
}


void
Skin::BuildLetterMap (void)
{
	QPixmap *img = getPixmap("text");

	if(img)
	{
		QList<QList<QPixmap> >(letters);
		for (int i = 0; i < 3; i++) {
			QList<QPixmap>(l);
			for (int j = 0; j < 31; j++) {
				l.append(img->copy(j*5, i*6, 5, 6));
			}
			letters.append(l);
		}
	
		delete img;
	
	
		/* alphabet */
		for (uint i = 97; i < 123; i++) {
			m_letterMap[i] = letters[0][i-97];
		}
	
		/* digits */
		for (uint i = 0; i <= 9; i++) {
			m_letterMap[i+48] = letters[1][i];
		}
	
		/* special characters */
		m_letterMap['"'] = letters[0][27];
		m_letterMap['@'] = letters[0][28];
		m_letterMap['('] = letters[1][13];
		m_letterMap[')'] = letters[1][14];
		m_letterMap['-'] = letters[1][15];
		m_letterMap['\''] = letters[1][16];
		m_letterMap['`'] = letters[1][16];
		m_letterMap['!'] = letters[1][17];
		m_letterMap['_'] = letters[1][18];
		m_letterMap['+'] = letters[1][19];
		m_letterMap['\\'] = letters[1][20];
		m_letterMap['/'] = letters[1][21];
		m_letterMap['['] = letters[1][22];
		m_letterMap[']'] = letters[1][23];
		m_letterMap['^'] = letters[1][24];
		m_letterMap['&'] = letters[1][25];
		m_letterMap['%'] = letters[1][26];
		m_letterMap['.'] = letters[1][27];
		m_letterMap[','] = letters[1][27];
		m_letterMap['='] = letters[1][28];
		m_letterMap['$'] = letters[1][29];
		m_letterMap['#'] = letters[1][30];
	
		m_letterMap[229] = letters[2][0];
		m_letterMap[246] = letters[2][1];
		m_letterMap[228] = letters[2][2];
		m_letterMap['?'] = letters[2][3];
		m_letterMap['*'] = letters[2][4];
		m_letterMap[' '] = letters[2][5];
	
		/* text background */
		m_items[TEXTBG] = letters[2][6];
	}
	else
		setSkin(":CleanAMP/");
}


void
Skin::BuildButtons (void)
{
	QPixmap *img = getPixmap("cbuttons");
	if(img)
	{
		QIcon icon;
		icon.addPixmap (img->copy (0,  0, 23, 18), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (0, 18, 23, 18), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_PREV] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (23,  0, 23, 18), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (23, 18, 23, 18), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_PLAY] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (46,  0, 23, 18), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (46, 18, 23, 18), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_PAUSE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (69,  0, 23, 18), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (69, 18, 23, 18), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_STOP] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (92,  0, 22, 18), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (92, 18, 22, 18), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_NEXT] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (114,  0, 22, 16), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (114, 16, 22, 16), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_EJECT] = icon;

		delete img;
	}
	else
		setSkin(":CleanAMP/");
}


void
Skin::BuildToggleButtons (void)
{
	QPixmap *img = getPixmap("shufrep");

	if(img)
	{
		QIcon icon;
		icon.addPixmap (img->copy ( 0, 61, 23, 12), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (46, 61, 23, 12), QIcon::Active, QIcon::Off);
		icon.addPixmap (img->copy ( 0, 73, 23, 12), QIcon::Normal, QIcon::On);
		icon.addPixmap (img->copy (46, 73, 23, 12), QIcon::Active, QIcon::On);
		m_icons[BUTTON_MW_EQ] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (23, 61, 23, 12), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (69, 61, 23, 12), QIcon::Active, QIcon::Off);
		icon.addPixmap (img->copy (23, 73, 23, 12), QIcon::Normal, QIcon::On);
		icon.addPixmap (img->copy (69, 73, 23, 12), QIcon::Active, QIcon::On);
		m_icons[BUTTON_MW_PLS] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (28,  0, 46, 15), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (28, 15, 46, 15), QIcon::Active, QIcon::Off);
		icon.addPixmap (img->copy (28, 30, 46, 15), QIcon::Normal, QIcon::On);
		icon.addPixmap (img->copy (28, 45, 46, 15), QIcon::Active, QIcon::On);
		m_icons[BUTTON_MW_SHUFFLE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (0,  0, 28, 15), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (0, 15, 28, 15), QIcon::Active, QIcon::Off);
		icon.addPixmap (img->copy (0, 30, 28, 15), QIcon::Normal, QIcon::On);
		icon.addPixmap (img->copy (0, 45, 28, 15), QIcon::Active, QIcon::On);
		m_icons[BUTTON_MW_REPEAT] = icon;

		delete img;
	}
	else
		setSkin(":CleanAMP/");
}


void
Skin::BuildTitleBar (void)
{
	QPixmap *img = getPixmap("titlebar");

	if(img)
	{
		QIcon icon;
		icon.addPixmap (img->copy (0, 0, 9, 9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (0, 9, 9, 9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_MENU] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (9, 0, 9, 9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (9, 9, 9, 9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_MINIMIZE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (18, 0, 9, 9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (18, 9, 9, 9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_CLOSE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (0, 18, 9, 9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (9, 18, 9, 9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_SHADE] = icon;

		icon = QIcon ();
		icon.addPixmap (img->copy (0, 27, 9, 9), QIcon::Normal, QIcon::Off);
		icon.addPixmap (img->copy (9, 27, 9, 9), QIcon::Active, QIcon::Off);
		m_icons[BUTTON_MW_SHADED_UNSHADE] = icon;

		m_items[TITLEBAR_0] = img->copy(27, 0, 275, 14);
		m_items[TITLEBAR_1] = img->copy(27, 15, 275, 14);
	
		m_items[STATUSBAR_0] = img->copy(27, 29, 275, 14);
		m_items[STATUSBAR_1] = img->copy(27, 29+13, 275, 14);
	
		m_items[CLUTTER_ON]  = img->copy(304+8*0, 0, 8, 43);
		m_items[CLUTTER_OFF] = img->copy(304+8*1, 0, 8, 43);

		m_items[CLUTTER_O] = img->copy(304+8*0, 44, 8, 43);
		m_items[CLUTTER_A] = img->copy(304+8*1, 44, 8, 43);
		m_items[CLUTTER_I] = img->copy(304+8*2, 44, 8, 43);
		m_items[CLUTTER_D] = img->copy(304+8*3, 44, 8, 43);
		m_items[CLUTTER_V] = img->copy(304+8*4, 44, 8, 43);

		delete img;
	}
	else
		setSkin(":CleanAMP/");
}


void
Skin::BuildOther (void)
{
	QPixmap *img, *part;
	QPainter(painter);
	
	img = getPixmap("monoster");
	if(img)
	{
		m_items[MONO_1] = img->copy(29,  0, 27, 12);
		m_items[MONO_0] = img->copy(29, 12, 27, 12);
		m_items[STEREO_1] = img->copy(0,  0, 29, 12);
		m_items[STEREO_0] = img->copy(0, 12, 29, 12);
		delete img;
	}
	else
		setSkin(":CleanAMP/");

	img = getPixmap("playpaus");
	if(img)
	{
		part = new QPixmap(11, 9);
		painter.begin(part);
		painter.drawPixmap (0, 0, 3, 9, *img, 36, 0, 3, 9);
		painter.drawPixmap (3, 0, 8, 9, *img,  1, 0, 8, 9);
		painter.end();
		m_items[PIC_PLAY] = part->copy();
		delete part;

		part = new QPixmap(11, 9);
		painter.begin(part);
		painter.drawPixmap (0, 0, 2, 9, *img, 27, 0, 2, 9);
		painter.drawPixmap (2, 0, 9, 9, *img,  9, 0, 9, 9);
		painter.end();
		m_items[PIC_PAUSE] = part->copy();
		delete part;
	
		part = new QPixmap(11, 9);
		painter.begin(part);
		painter.drawPixmap (0, 0, 2, 9, *img, 27, 0, 2, 9);
		painter.drawPixmap (2, 0, 9, 9, *img, 18, 0, 9, 9);
		painter.end();
		m_items[PIC_STOP] = part->copy();
		delete part;
	
		delete img;
 	}
	else
		setSkin(":CleanAMP/");

	img = getPixmap ("main");
	if(img)
	{
		m_items[MAIN_WINDOW] = img->copy();
		m_items[ABOUT_0] = img->copy(247, 83, 20, 25);
		m_items[ABOUT_1] = img->copy(247, 83, 20, 24);
		delete img;
	}
	else
		setSkin(":CleanAMP/");
}


void
Skin::BuildSliders (void)
{
	QPixmap *img;

	img = getPixmap("posbar");
	if (img) {
		m_sizes[SLIDER_POSBAR_BGS] = QSize (248, qMin (10, img->height ()));
		m_items[POSBAR] = img->copy (0, 0, 248, qMin (10, img->height ()));
		m_items[POSBAR_BTN_0] = img->copy (248, 0, 29, qMin (10, img->height ()));
		m_items[POSBAR_BTN_1] = img->copy (278, 0, 29, qMin (10, img->height ()));

		delete img;
	} else {
		setSkin(":CleanAMP/");
	}

	img = getPixmap("volume");
	if (img) {
		QPixmapList list;
		for (int i = 0; i <= 27; i++) {
			list << img->copy(0, i*15, 68, 13);
		}
		m_backgrounds[SLIDER_VOLUMEBAR_BGS] = list;

		if (img->height() > 421) {
			m_items[VOLBAR_BTN_1] = img->copy (0, 422, 14, qMin (11, img->height () - 422));
			m_items[VOLBAR_BTN_0] = img->copy (15, 422, 14, qMin (11, img->height () - 422));
		}
	
		delete img;
	} else {
		setSkin(":CleanAMP/");
	}

	img = getPixmap("balance");
	if (!img) {
		img = getPixmap("volume");
	}

	if (img) {
		QPixmapList list;
		QPixmap p;
		list << img->copy (9, 15, 38, 13);
		for (int i = 1; i < 28; i++) {
			// use p to make use of Qt implicit sharing (I think it will not
			// work if appended and prepended Images are copied seperately)
			p = img->copy(9, i*15, 38, 13);
			list.append (p);
			list.prepend (p);
		}
		m_backgrounds[SLIDER_BALANCEBAR_BGS] = list;

		if (img->height() > 421) {
			m_items[BALANCE_BTN_1] = img->copy(0, 422, 14, qMin (11, img->height () - 422));
			m_items[BALANCE_BTN_0] = img->copy(15, 422, 14, qMin (11, img->height () - 422));
		}

		delete img;
	} else {
		setSkin(":CleanAMP/");
	}
}


void
Skin::BuildNumbers (void)
{
	uint num = 12;

	QPixmap *img = getPixmap("nums_ex");
	if (!img) {
		num = 11;
		img = getPixmap ("numbers");
	}

	if(img)
	{
		for (uint i = 0; i < num; i++) {
			m_numbers[i] = img->copy (i*9, 0, 9, qMin (13, img->height ()));
		}
		delete img;
	}
	else
		setSkin(":CleanAMP/");

}


void
Skin::ParsePLEdit (void)
{
	QDir dir;
	QString path;

	dir.setPath (m_path);
	dir.setFilter (QDir::Files);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName().toLower() == "pledit.txt") {
			path += fileInfo.filePath ();
			break;
		}
	}

	if (path.isNull ()) {
		qDebug ("trasigt!");
		return;
	}

	QFile file (path);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	while (!file.atEnd ()) {
		QByteArray line = file.readLine ();
		QList<QByteArray> l = line.split ('=');
		if (l.count () == 2) {
			m_pledit_txt[l[0].toLower ()] = l[1].trimmed();
		} else if (line.length() == 0) {
			break;
		}
	}

}

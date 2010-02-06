/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2010 XMMS2 Team
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

#include "Skin.h"

#include <QDir>
#include <QPainter>
#include <QSettings>

Skin::Skin (const QString &url)
{
	setSizes ();
	setPositions ();

	m_valid = setSkin (url);
}

// Copy string and replace data with that from url.
Skin::Skin (Skin *other, const QString &url)
{
	Q_ASSERT (other != 0);

	m_sizes     = other->m_sizes;
	m_positions = other->m_positions;

	// At the moment, these are only used if url == "", but
	// later windows that aren't modified in a skin should be displayed
	// with the default skin
	m_items        = other->m_items;
	m_letterMap    = other->m_letterMap;
	m_smallNumbers = other->m_smallNumbers;
	m_numbers      = other->m_numbers;
	m_playlist     = other->m_playlist;
	m_pledit_txt   = other->m_pledit_txt;

	// TODO: Use default skin for missing information
	if (!url.isEmpty ()) {
		m_valid = setSkin (url);
	} else {
		m_valid = true;
	}
}

/*
 * handler methods
 */

bool
Skin::handle_main (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	m_items[MAIN_WINDOW] = img;
	m_items[ABOUT_0] = img.copy(247, 83, 20, 25);
	m_items[ABOUT_1] = img.copy(247, 83, 20, 24);

	return true;
}

bool
Skin::handle_titlebar (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	QIcon icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (0, 0, 9, 9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (0, 9, 9, 9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_MENU] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (9, 0, 9, 9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (9, 9, 9, 9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_MINIMIZE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (18, 0, 9, 9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (18, 9, 9, 9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_CLOSE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (0, 18, 9, 9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (9, 18, 9, 9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_SHADE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (0, 27, 9, 9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (9, 27, 9, 9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_SHADED_UNSHADE] = icon;

	m_items[TITLEBAR_0] = img.copy(27, 0, 275, 14);
	m_items[TITLEBAR_1] = img.copy(27, 15, 275, 14);

	m_items[STATUSBAR_0] = img.copy(27, 29, 275, 14);
	m_items[STATUSBAR_1] = img.copy(27, 29+13, 275, 14);

	m_items[CLUTTER_ON]  = img.copy(304+8*0, 0, 8, 43);
	m_items[CLUTTER_OFF] = img.copy(304+8*1, 0, 8, 43);

	m_items[CLUTTER_O] = img.copy(304+8*0, 44, 8, 43);
	m_items[CLUTTER_A] = img.copy(304+8*1, 44, 8, 43);
	m_items[CLUTTER_I] = img.copy(304+8*2, 44, 8, 43);
	m_items[CLUTTER_D] = img.copy(304+8*3, 44, 8, 43);
	m_items[CLUTTER_V] = img.copy(304+8*4, 44, 8, 43);

	return true;
}

bool
Skin::handle_posbar (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	const int h = qMin (10, img.height ());
	m_sizes[SLIDER_POSBAR_BGS] = QSize (248, h);
	m_items[POSBAR] = img.copy (0, 0, 248, h);
	m_items[POSBAR_BTN_0] = img.copy (248, 0, 29, h);
	m_items[POSBAR_BTN_1] = img.copy (278, 0, 29, h);
}

bool
Skin::handle_volume (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	QPixmapList list;
	for (int i = 0; i <= 27; i++) {
		list << img.copy(0, i*15, qMin (68, img.width()), 13);
	}
	m_backgrounds[SLIDER_VOLUMEBAR_BGS] = list;

	if (img.height() > 421) {
		m_items[VOLBAR_BTN_1] = img.copy (0, 422, 14,
		                                  qMin (11, img.height () - 422));
		m_items[VOLBAR_BTN_0] = img.copy (15, 422, 14,
		                                  qMin (11, img.height () - 422));
	}

	return true;
}

bool
Skin::handle_balance (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	QPixmap p;
	QPixmapList list;

	list << img.copy (9, 15, 38, 13);
	for (int i = 1; i < 28; i++) {
		// use p to make use of Qt implicit sharing (I think it will not
		// work if appended and prepended Images are copied seperately)
		p = img.copy(9, i*15, 38, 13);
		list.append (p);
		list.prepend (p);
	}
	m_backgrounds[SLIDER_BALANCEBAR_BGS] = list;

	if (img.height() > 421) {
		m_items[BALANCE_BTN_1] = img.copy(0, 422, 14,
		                                  qMin (11, img.height () - 422));
		m_items[BALANCE_BTN_0] = img.copy(15, 422, 14,
		                                  qMin (11, img.height () - 422));
	}

	return true;
}

bool
Skin::handle_cbuttons (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	QIcon icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (0,  0, 23, 18), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (0, 18, 23, 18), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_PREV] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (23,  0, 23, 18), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (23, 18, 23, 18), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_PLAY] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (46,  0, 23, 18), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (46, 18, 23, 18), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_PAUSE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (69,  0, 23, 18), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (69, 18, 23, 18), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_STOP] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (92,  0, 22, 18), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (92, 18, 22, 18), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_NEXT] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (114,  0, 22, 16), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (114, 16, 22, 16), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_MW_EJECT] = icon;

	return true;
}

bool
Skin::handle_monoster (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	m_items[MONO_1] = img.copy(29,  0, 27, 12);
	m_items[MONO_0] = img.copy(29, 12, 27, 12);
	m_items[STEREO_1] = img.copy(0,  0, 29, 12);
	m_items[STEREO_0] = img.copy(0, 12, 29, 12);

	return true;
}

bool
Skin::handle_playpaus (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	QPixmap part;
	QPainter painter;

	const int h = qMin (img.height (), 9);
	part = QPixmap(11, h);
	painter.begin(&part);
	painter.drawPixmap (0, 0, 3, h, img, 36, 0, 3, h);
	painter.drawPixmap (3, 0, 8, h, img,  1, 0, 8, h);
	painter.end();
	m_items[PIC_PLAY] = part;

	part = QPixmap(11, h);
	painter.begin(&part);
	painter.drawPixmap (0, 0, 2, h, img, 27, 0, 2, h);
	painter.drawPixmap (2, 0, 9, h, img,  9, 0, 9, h);
	painter.end();
	m_items[PIC_PAUSE] = part;

	part = QPixmap(11, h);
	painter.begin(&part);
	painter.drawPixmap (0, 0, 2, h, img, 27, 0, 2, h);
	painter.drawPixmap (2, 0, 9, h, img, 18, 0, 9, h);
	painter.end();
	m_items[PIC_STOP] = part;

	return true;
}

bool
Skin::handle_shufrep (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	QIcon icon;

	icon = QIcon ();
	icon.addPixmap (img.copy ( 0, 61, 23, 12), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (46, 61, 23, 12), QIcon::Active, QIcon::Off);
	icon.addPixmap (img.copy ( 0, 73, 23, 12), QIcon::Normal, QIcon::On);
	icon.addPixmap (img.copy (46, 73, 23, 12), QIcon::Active, QIcon::On);
	m_icons[BUTTON_MW_EQ] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (23, 61, 23, 12), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (69, 61, 23, 12), QIcon::Active, QIcon::Off);
	icon.addPixmap (img.copy (23, 73, 23, 12), QIcon::Normal, QIcon::On);
	icon.addPixmap (img.copy (69, 73, 23, 12), QIcon::Active, QIcon::On);
	m_icons[BUTTON_MW_PLS] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (28,  0, 46, 15), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (28, 15, 46, 15), QIcon::Active, QIcon::Off);
	icon.addPixmap (img.copy (28, 30, 46, 15), QIcon::Normal, QIcon::On);
	icon.addPixmap (img.copy (28, 45, 46, 15), QIcon::Active, QIcon::On);
	m_icons[BUTTON_MW_SHUFFLE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (0,  0, 28, 15), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (0, 15, 28, 15), QIcon::Active, QIcon::Off);
	icon.addPixmap (img.copy (0, 30, 28, 15), QIcon::Normal, QIcon::On);
	icon.addPixmap (img.copy (0, 45, 28, 15), QIcon::Active, QIcon::On);
	m_icons[BUTTON_MW_REPEAT] = icon;

	return true;
}

bool
Skin::handle_text (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	QList<QList<QPixmap> >(letters);
	for (int i = 0; i < 3; i++) {
		QList<QPixmap>(l);
		for (int j = 0; j < 31; j++) {
			l.append(img.copy(j*5, i*6, 5, 6));
		}
		letters.append(l);
	}

	/* alphabet */
	for (uint i = 97; i < 123; i++) {
		m_letterMap[i] = letters[0][i-97];
	}

	/* digits */
	for (uint i = 0; i <= 9; i++) {
		m_letterMap[i+48] = letters[1][i];
		m_smallNumbers [i] = letters[1][i];
	}

	/* special characters */
	m_letterMap['"'] = letters[0][27];
	m_letterMap['@'] = letters[0][28];
	m_letterMap[':'] = letters[1][12];
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

	m_smallNumbers[10] = letters[2][6];
	m_smallNumbers[-1] = m_smallNumbers[10]; // also add Blank to index -1
	m_smallNumbers[11] = letters[1][15];

	return true;
}

bool
Skin::handle_numbers (const QPixmap &img)
{
	// This method handles both nums_ex and the fallback numbers
	if (img.isNull ())
		return false;

	const int num = (img.width () >= 9 * 12) ? 12 : 11;

	for (int i = 0; i < num; i++) {
		m_numbers[i] = img.copy (i*9, 0, 9, qMin (13, img.height ()));
	}
	m_numbers[-1] = m_numbers[10]; // add Blank space to index -1

	if (num < 12) {
		// We do not yet have a '-' Symbol, so we create one
		// from the '2' character and the blank as background
		QPixmap pix = m_numbers[10].copy ();
		QRect r (3, 6, 3, 1);
		QPainter painter (&pix);
		painter.drawPixmap (r, m_numbers[2], r);
		m_numbers[11] = pix;
	}

	return true;
}

bool
Skin::handle_eqmain (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	m_items[EQ_WIN_BG] = img.copy (0, 0, 275, 116);

	if (img.height () > 294) {
		m_items[EQ_WIN_GRAPH_BG] =
			img.copy (0, 294, 113, qMin (19, img.height () - 294));
	} else {
		m_items[EQ_WIN_GRAPH_BG] = QPixmap ();
	}

	QIcon icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (  0, 116,  9,  9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (  0, 125,  9,  9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_EQ_CLOSE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy ( 10, 119, 25, 12), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (128, 119, 25, 12), QIcon::Active, QIcon::Off);
	icon.addPixmap (img.copy ( 69, 119, 25, 12), QIcon::Normal, QIcon::On);
	icon.addPixmap (img.copy (187, 119, 25, 12), QIcon::Active, QIcon::On);
	m_icons[BUTTON_EQ_ACTIVE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy ( 35, 119, 33, 12), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (153, 119, 33, 12), QIcon::Active, QIcon::Off);
	icon.addPixmap (img.copy ( 94, 119, 33, 12), QIcon::Normal, QIcon::On);
	icon.addPixmap (img.copy (212, 119, 33, 12), QIcon::Active, QIcon::On);
	m_icons[BUTTON_EQ_AUTO] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (224, 164, 44, 12), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (224, 176, 44, 12), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_EQ_PRESET] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (254, 3,  9,  9), QIcon::Normal, QIcon::Off);
	m_icons[BUTTON_EQ_SHADE] = icon;

	QPixmapList list;
	for (int i = 0; i < 14; i++) {
		list << img.copy (13+15*i, 164, 14, 63);
	}
	for (int i = 0; i < 14; i++) {
		list << img.copy (13+15*i, 229, 14, 63);
	}
	m_backgrounds[SLIDER_EQUALIZER_BGS] = list;

	m_items[EQ_WIN_BAR_BTN_0] = img.copy (0, 164, 11, 11);
	m_items[EQ_WIN_BAR_BTN_1] = img.copy (0, 176, 11, 11);

	return true;
}

bool
Skin::handle_eq_ex (const QPixmap &img)
{
	// This method has to be called after handle_eqmain!
	if (img.isNull ())
		return false;

	m_icons[BUTTON_EQ_SHADE].addPixmap (img.copy ( 1, 38,  9,  9),
	                                    QIcon::Active, QIcon::Off);

	return true;
}

bool
Skin::handle_pledit (const QPixmap &img)
{
	if (img.isNull ())
		return false;

	QIcon icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (167,  3,  9,  9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy ( 52, 42,  9,  9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_PLS_CLOSE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (158,  3,  9,  9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy ( 62, 42,  9,  9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_PLS_SHADE] = icon;

	icon = QIcon ();
	icon.addPixmap (img.copy (129, 45,  9,  9), QIcon::Normal, QIcon::Off);
	icon.addPixmap (img.copy (150, 42,  9,  9), QIcon::Active, QIcon::Off);
	m_icons[BUTTON_PLS_SHADED_UNSHADE] = icon;

	m_playlist[PLS_CORNER_UL_0] = img.copy(0, 0, 25, 20);
	m_playlist[PLS_CORNER_UL_1] = img.copy(0, 21, 25, 20);

	m_playlist[PLS_TBAR_0] = img.copy (26, 0, 100, 20);
	m_playlist[PLS_TBAR_1] = img.copy (26, 21, 100, 20);

	m_playlist[PLS_CORNER_UR_0] = img.copy(153, 0, 25, 20);
	m_playlist[PLS_CORNER_UR_1] = img.copy(153, 21, 25, 20);

	m_playlist[PLS_TFILL_0] = img.copy(127, 0, 25, 20);
	m_playlist[PLS_TFILL_1] = img.copy(127, 21, 25, 20);

	m_playlist[PLS_BFILL_0] = img.copy(179, 0, 25, 38);

	m_playlist[PLS_VISMINI_0] = img.copy(205, 0, 75, 38);

	m_playlist[PLS_LFILL_0] = img.copy(0, 42, 12, 29);

	m_playlist[PLS_RFILL_0] = img.copy(31, 42, 5, 29);
	m_playlist[PLS_RFILL2_0] = img.copy(36, 42, 8, 29);
	m_playlist[PLS_RFILL3_0] = img.copy(44, 42, 7, 29);

	m_playlist[PLS_MAX_BTN_0] = img.copy(150, 42, 9, 9);

	m_playlist[PLS_SCROLL_0] = img.copy(52, 53, 8, 18);
	m_playlist[PLS_SCROLL_1] = img.copy(61, 53, 8, 18);

	m_playlist[PLS_WS_LE_0] = img.copy(72, 42, 25, 14);
	m_playlist[PLS_WS_RE_0] = img.copy(99, 42, 50, 14);
	m_playlist[PLS_WS_RE_1] = img.copy(99, 57, 50, 14);
	m_playlist[PLS_WS_MID_0] = img.copy(72, 57, 25, 14);

	m_playlist[PLS_LCBAR] = img.copy(0, 72, 125, 38);
	m_playlist[PLS_RCBAR] = img.copy(126, 72, 150, 38);

	/* extract the buttons */
	m_playlist[PLS_ADD] = img.copy(11, 80, 25, 18);
	m_playlist[PLS_ADD_DEC] = img.copy(48, 111, 3, 54);
	m_playlist[PLS_ADD_URL_0] = img.copy(0, 111, 22, 18);
	m_playlist[PLS_ADD_URL_1] = img.copy(23, 111, 22, 18);

	m_playlist[PLS_ADD_DIR_0] = img.copy(0, 130, 22, 18);
	m_playlist[PLS_ADD_DIR_1] = img.copy(23, 130, 22, 18);

	m_playlist[PLS_ADD_FIL_0] = img.copy(0, 149, 22, 18);
	m_playlist[PLS_ADD_FIL_1] = img.copy(23, 149, 22, 18);

	/* Delete buttons */
	m_playlist[PLS_DEL] = img.copy(40, 80, 25, 18);
	m_playlist[PLS_DEL_DEC] = img.copy(100, 111, 3, 72);
	m_playlist[PLS_DEL_ALL_0] = img.copy(54, 111, 22, 18);
	m_playlist[PLS_DEL_ALL_1] = img.copy(77, 111, 22, 18);

	m_playlist[PLS_DEL_CRP_0] = img.copy(54, 130, 22, 18);
	m_playlist[PLS_DEL_CRP_1] = img.copy(77, 130, 22, 18);

	m_playlist[PLS_DEL_FIL_0] = img.copy(54, 149, 22, 18);
	m_playlist[PLS_DEL_FIL_1] = img.copy(77, 149, 22, 18);

	/* Select buttons */
	m_playlist[PLS_SEL] = img.copy(69, 80, 25, 18);
	m_playlist[PLS_SEL_DEC] = img.copy(150, 111, 3, 54);
	m_playlist[PLS_SEL_INV_0] = img.copy(104, 111, 22, 18);
	m_playlist[PLS_SEL_INV_1] = img.copy(127, 111, 22, 18);

	m_playlist[PLS_SEL_NIL_0] = img.copy(104, 130, 22, 18);
	m_playlist[PLS_SEL_NIL_1] = img.copy(127, 130, 22, 18);

	m_playlist[PLS_SEL_ALL_0] = img.copy(104, 149, 22, 18);
	m_playlist[PLS_SEL_ALL_1] = img.copy(127, 149, 22, 18);

	/* misc buttons */
	m_playlist[PLS_MSC] = img.copy(98, 80, 25, 18);
	m_playlist[PLS_MSC_DEC] = img.copy(200, 111, 3, 54);
	m_playlist[PLS_MSC_SRT_0] = img.copy(154, 111, 22, 18);
	m_playlist[PLS_MSC_SRT_1] = img.copy(177, 111, 22, 18);

	m_playlist[PLS_MSC_INF_0] = img.copy(154, 130, 22, 18);
	m_playlist[PLS_MSC_INF_1] = img.copy(177, 130, 22, 18);

	m_playlist[PLS_MSC_OPT_0] = img.copy(154, 149, 22, 18);
	m_playlist[PLS_MSC_OPT_1] = img.copy(177, 149, 22, 18);

	/* list buttons */
	m_playlist[PLS_LST] = img.copy(229, 80, 25, 18);
	m_playlist[PLS_LST_DEC] = img.copy(250, 111, 3, 54);
	m_playlist[PLS_LST_NEW_0] = img.copy(204, 111, 22, 18);
	m_playlist[PLS_LST_NEW_1] = img.copy(227, 111, 22, 18);

	m_playlist[PLS_LST_SAV_0] = img.copy(204, 130, 22, 18);
	m_playlist[PLS_LST_SAV_1] = img.copy(227, 130, 22, 18);

	m_playlist[PLS_LST_OPN_0] = img.copy(204, 149, 22, 18);
	m_playlist[PLS_LST_OPN_1] = img.copy(227, 149, 22, 18);

	/* misc button */
	m_playlist[PLS_MSC_BTN_0] = img.copy(54, 168, 22, 18);
	m_playlist[PLS_MSC_BTN_1] = img.copy(77, 168, 22, 18);
	return true;
}

bool
Skin::handle_pledit_txt (QIODevice *device)
{
	Q_ASSERT (device != 0);

	device->setTextModeEnabled (true);

	while (!device->atEnd ()) {
		QByteArray line = device->readLine ().trimmed ();
		if (line.length () == 0) {
			continue;
		}

		QList<QByteArray> l = line.split ('=');
		if (l.count () == 2) {
			m_pledit_txt[l[0].toLower ().trimmed ()] = l[1].trimmed();
		}
	}

	return true;
}

/*
 * ---legacy code start ---
 */

bool
Skin::setSkin (const QString& path)
{
	QDir dir(path);
	if (!dir.exists ())
		return false;

	m_path = path;

	m_items.clear();
	m_letterMap.clear();
	m_smallNumbers.clear ();
	m_numbers.clear();
	m_pledit_txt.clear();
	m_playlist.clear ();

	bool b_main = false, b_titlebar = false, b_posbar = false, b_volume = false;
	bool b_balance = false, b_cbuttons = false, b_monoster = false;
	bool b_playpaus = false, b_shufrep = false, b_text = false;
	bool b_numbers = false, b_eqmain = false, b_pledit = false;
	bool b_pledit_txt = false;

	QPixmap p_eq_ex;
	QPixmap p_numbers;
	QPixmap p_volume;

	dir.setFilter (QDir::Files|QDir::NoDotAndDotDot);
	QStringList entries = dir.entryList ();
	for (int i = 0; i < entries.size (); i++) {
		QString filename = entries.at (i);
		QString entry = filename.toLower ();
		if (entry.endsWith (".txt")) {
			QFile f (dir.absoluteFilePath (filename));
			if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
				continue;
			if ( entry == "pledit.txt") {
				b_pledit_txt = handle_pledit_txt (&f);
			}
			continue;
		} else if (entry.endsWith (".cur")) {
			// Cursor files are not supported yet
			continue;
		} else {
			QPixmap img = QPixmap (dir.absoluteFilePath (filename));
			if (img.isNull ()) {
				continue;
			}

			entry = entry.section(".", 0, 0);
			if (entry == "main") {
				b_main = handle_main (img);
			} else if (entry == "titlebar") {
				b_titlebar = handle_titlebar (img);
			} else if (entry == "posbar" ) {
				b_posbar = handle_posbar (img);
			} else if (entry == "volume") {
				p_volume = img;
				b_volume = handle_volume (img);
			} else if (entry == "balance") {
				b_balance = handle_balance (img);
			} else if (entry == "cbuttons") {
				b_cbuttons = handle_cbuttons (img);
			} else if (entry == "monoster") {
				b_monoster = handle_monoster (img);
			} else if (entry == "playpaus") {
				b_playpaus = handle_playpaus (img);
			} else if (entry == "shufrep") {
				b_shufrep = handle_shufrep (img);
			} else if (entry == "text") {
				b_text = handle_text (img);
			} else if (entry == "nums_ex") {
				b_numbers = handle_numbers (img);
			} else if (entry == "numbers") {
				p_numbers = img;
			} else if (entry == "eqmain") {
				b_eqmain = handle_eqmain (img);
			} else if (entry == "eq_ex") {
				p_eq_ex = img;
			} else if (entry == "pledit") {
				b_pledit = handle_pledit (img);
			}
			continue;
		}
	}

	if (!b_balance) {
		// Fallback, use volume image if skin has no balance image
		handle_balance (p_volume);
	}
	if (!b_numbers) {
		// Fallback if skin has no nums_ex file
		b_numbers = handle_numbers (p_numbers);
	}

	// handle_eq_ex must be called after handle_eqmain
	if (b_eqmain) {
		handle_eq_ex (p_eq_ex);
	}

	return (b_main && b_titlebar && b_posbar && b_volume && b_cbuttons &&
	        b_monoster && b_playpaus && b_shufrep && b_text && b_numbers &&
	        b_eqmain && b_pledit && b_pledit_txt);
}

const QPixmap
Skin::getPixmap (const QString& file, const QString &path)
{
	QDir dir (path);
	dir.setFilter (QDir::Files|QDir::NoDotAndDotDot);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QString fname = fileInfo.fileName().toLower();
		if (fname.section(".", 0, 0) == file) {
			return QPixmap (fileInfo.filePath());
		}
	}

	return QPixmap ();
}


/*
 * Initializer functions for sizes and positions
 */

void
Skin::setSizes ()
{
	m_sizes.clear ();
	/* Please keep this list sorted like the enum */
	m_sizes.insert (BUTTON_MW_MENU, QSize (9, 9));
	m_sizes.insert (BUTTON_MW_MINIMIZE, QSize (9, 9));
	m_sizes.insert (BUTTON_MW_CLOSE, QSize (9, 9));
	m_sizes.insert (BUTTON_MW_SHADE, QSize (9, 9));
	m_sizes.insert (BUTTON_MW_PREV, QSize (23, 18));
	m_sizes.insert (BUTTON_MW_PLAY, QSize (23, 18));
	m_sizes.insert (BUTTON_MW_PAUSE, QSize (23, 18));
	m_sizes.insert (BUTTON_MW_STOP, QSize (23, 18));
	m_sizes.insert (BUTTON_MW_NEXT, QSize (22, 18));
	m_sizes.insert (BUTTON_MW_EJECT, QSize (22, 16));
	m_sizes.insert (BUTTON_MW_EQ, QSize (23, 12));
	m_sizes.insert (BUTTON_MW_PLS, QSize (23, 12));
	m_sizes.insert (BUTTON_MW_SHUFFLE, QSize (46, 15));
	m_sizes.insert (BUTTON_MW_REPEAT, QSize (28, 15));
	m_sizes.insert (BUTTON_MW_SHADED_UNSHADE, QSize (9, 9));
	m_sizes.insert (BUTTON_EQ_CLOSE, QSize (9, 9));
	m_sizes.insert (BUTTON_EQ_SHADE, QSize (9, 9));
	m_sizes.insert (BUTTON_EQ_ACTIVE, QSize (25, 12));
	m_sizes.insert (BUTTON_EQ_AUTO, QSize (33, 12));
	m_sizes.insert (BUTTON_EQ_PRESET, QSize (44, 12));
//	m_sizes.insert (BUTTON_EQ_SHADED_CLOSE, QSize ());
//	m_sizes.insert (BUTTON_EQ_SHADED_UNSHADE, QSize ());
	m_sizes.insert (BUTTON_PLS_CLOSE, QSize (9, 9));
	m_sizes.insert (BUTTON_PLS_SHADE, QSize (9, 9));
	m_sizes.insert (BUTTON_PLS_SHADED_UNSHADE, QSize (9, 9));
//	m_sizes.insert (SLIDER_POSBAR, QSize ());
	m_sizes.insert (SLIDER_POSBAR_BGS, QSize (248, 10));
	m_sizes.insert (SLIDER_VOLUMEBAR, QSize (14, 11));
	m_sizes.insert (SLIDER_VOLUMEBAR_BGS, QSize (68, 13));
//	m_sizes.insert (SLIDER_BALANCEBAR, QSize ());
	m_sizes.insert (SLIDER_BALANCEBAR_BGS, QSize (38, 13));
//	m_sizes.insert (SLIDER_EQUALIZER, QSize ());
	m_sizes.insert (SLIDER_EQUALIZER_BGS, QSize (14, 63));
}

void
Skin::setPositions ()
{
	m_positions.clear ();
	/* Please keep this list sorted like the enum */
	m_positions.insert (BUTTON_MW_MENU, QPoint (6, 3));
	m_positions.insert (BUTTON_MW_MINIMIZE, QPoint (244, 3));
	m_positions.insert (BUTTON_MW_CLOSE, QPoint (264, 3));
	m_positions.insert (BUTTON_MW_SHADE, QPoint (254, 3));
	m_positions.insert (BUTTON_MW_PREV, QPoint (16, 88));
	m_positions.insert (BUTTON_MW_PLAY, QPoint (39, 88));
	m_positions.insert (BUTTON_MW_PAUSE, QPoint (62, 88));
	m_positions.insert (BUTTON_MW_STOP, QPoint (85, 88));
	m_positions.insert (BUTTON_MW_NEXT, QPoint (108, 88));
	m_positions.insert (BUTTON_MW_EJECT, QPoint (136, 89));
	m_positions.insert (BUTTON_MW_EQ, QPoint (219, 58));
	m_positions.insert (BUTTON_MW_PLS, QPoint (242, 58));
	m_positions.insert (BUTTON_MW_SHUFFLE, QPoint (164, 89));
	m_positions.insert (BUTTON_MW_REPEAT, QPoint (210, 89));
	m_positions.insert (BUTTON_MW_SHADED_UNSHADE, QPoint (254, 3));
	m_positions.insert (BUTTON_EQ_CLOSE, QPoint (264, 3));
	m_positions.insert (BUTTON_EQ_SHADE, QPoint (254, 3));
	m_positions.insert (BUTTON_EQ_ACTIVE, QPoint (14, 18));
	m_positions.insert (BUTTON_EQ_AUTO, QPoint (39, 18));
	m_positions.insert (BUTTON_EQ_PRESET, QPoint (217, 18));
//	m_positions.insert (BUTTON_EQ_SHADED_CLOSE  , QPoint ());
//	m_positions.insert (BUTTON_EQ_SHADED_UNSHADE, QPoint ());
	/* The next 3 widgets are placed from the right */
	m_positions.insert (BUTTON_PLS_CLOSE, QPoint (-11, 3));
	m_positions.insert (BUTTON_PLS_SHADE, QPoint (-20, 3));
	m_positions.insert (BUTTON_PLS_SHADED_UNSHADE, QPoint (-20, 3));
//	m_positions.insert (SLIDER_POSBAR, QPoint ());
	m_positions.insert (SLIDER_POSBAR_BGS, QPoint (16, 72));
//	m_positions.insert (SLIDER_VOLUMEBAR, QPoint ());
	m_positions.insert (SLIDER_VOLUMEBAR_BGS, QPoint (107, 57));
//	m_positions.insert (SLIDER_BALANCEBAR, QPoint ());
	m_positions.insert (SLIDER_BALANCEBAR_BGS, QPoint (177, 57));
//	m_positions.insert (SLIDER_EQUALIZER, QPoint ());
//	m_positions.insert (SLIDER_EQUALIZER_BGS, QPoint ());
}

#include "Skin.moc"

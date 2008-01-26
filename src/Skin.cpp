/**
 *  This file is a part of Prome, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2007 XMMS2 Team
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

Skin *Skin::singleton = NULL;

Skin *Skin::getInstance (void)
{
	if (!singleton) {
		singleton = new Skin ();
	}

	return singleton;
}

void
Skin::BuildEqualizer (void)
{
	QPixmap *img = getPixmap ("eqmain");
	if (img) {
		m_items[EQ_WIN_BG] = img->copy (0, 0, 275, 116);
		m_items[EQ_WIN_ON_0] = img->copy (10, 119, 25, 12);
		m_items[EQ_WIN_ON_1] = img->copy (128, 119, 25, 12);
		m_items[EQ_WIN_OFF_1] = img->copy (187, 119, 25, 12);
		m_items[EQ_WIN_OFF_0] = img->copy (69, 119, 25, 12);

		m_items[EQ_WIN_AUTO_ON_0] = img->copy (35, 119, 33, 12);
		m_items[EQ_WIN_AUTO_ON_1] = img->copy (153, 119, 33, 12);
		m_items[EQ_WIN_AUTO_OFF_1] = img->copy (212, 119, 33, 12);
		m_items[EQ_WIN_AUTO_OFF_0] = img->copy (94, 119, 33, 12);

		m_items[EQ_WIN_PRESET_0] = img->copy (224, 164, 44, 12);
		m_items[EQ_WIN_PRESET_1] = img->copy (224, 176, 44, 12);

		m_items[EQ_WIN_GRAPH_BG] = img->copy (0, 294, 113, 19);

		for (int i = 0; i < 14; i++) {
			m_items[EQ_WIN_BAR_POS_0+i] = img->copy (13+15*i, 164, 14, 63);
		}
		for (int i = 0; i < 14; i++) {
			m_items[EQ_WIN_BAR_POS_14+i] = img->copy (13+15*i, 229, 14, 63);
		}

		m_items[EQ_WIN_BAR_BTN_1] = img->copy (0, 164, 11, 11);
		m_items[EQ_WIN_BAR_BTN_0] = img->copy (0, 176, 11, 11);

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
	
		m_playlist[PLS_CLOSE_BTN_0] = tmp.copy(14, 3, 9, 9);
		m_playlist[PLS_CLOSE_BTN_1] = img->copy(52, 42, 9, 9);
	
		m_playlist[PLS_SHADE_BTN_0] = tmp.copy(14, 3, 9, 9);
		m_playlist[PLS_SHADE_BTN_1] = img->copy(62, 42, 9, 9);
	
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
				l.append(img->copy(j*5, i*6, 4, 6));
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
		m_items[BTN_PREV_0] = img->copy(0,  0, 23, 18);
		m_items[BTN_PREV_1] = img->copy(0, 18, 23, 18);
	
		m_items[BTN_PLAY_0] = img->copy(23,  0, 23, 18);
		m_items[BTN_PLAY_1] = img->copy(23, 18, 23, 18);
	
		m_items[BTN_PAUSE_0] = img->copy(46,  0, 23, 18);
		m_items[BTN_PAUSE_1] = img->copy(46, 18, 23, 18);
	
		m_items[BTN_STOP_0] = img->copy(69,  0, 23, 18);
		m_items[BTN_STOP_1] = img->copy(69, 18, 23, 18);
	
		m_items[BTN_NEXT_0] = img->copy(92,  0, 22, 18);
		m_items[BTN_NEXT_1] = img->copy(92, 18, 22, 18);
	
		m_items[BTN_EJECT_0] = img->copy(114, 0, 22, 16);
		m_items[BTN_EJECT_1] = img->copy(114, 16, 22, 16);
	
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
		m_items[REPEAT_ON_0] = img->copy(0, 30, 28, 15);
		m_items[REPEAT_ON_1] = img->copy(0, 45, 28, 15);
	
		m_items[REPEAT_OFF_0] = img->copy(0,  0, 28, 15);
		m_items[REPEAT_OFF_1] = img->copy(0, 15, 28, 15);
	
		m_items[SHUFFLE_ON_0] = img->copy(28, 30, 46, 15);
		m_items[SHUFFLE_ON_1] = img->copy(28, 45, 46, 15);
	
		m_items[SHUFFLE_OFF_0] = img->copy(28,  0, 46, 15);
		m_items[SHUFFLE_OFF_1] = img->copy(28, 15, 46, 15);
	
		m_items[EQ_ON_0] = img->copy( 0, 73, 23, 12);
		m_items[EQ_ON_1] = img->copy(46, 73, 23, 12);
	
		m_items[EQ_OFF_0] = img->copy( 0, 61, 23, 12);
		m_items[EQ_OFF_1] = img->copy(46, 61, 23, 12);
	
		m_items[PLS_ON_0] = img->copy(23, 73, 23, 12);
		m_items[PLS_ON_1] = img->copy(69, 73, 23, 12);
	
		m_items[PLS_OFF_0] = img->copy(23, 61, 23, 12);
		m_items[PLS_OFF_1] = img->copy(69, 61, 23, 12);
	
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
		m_items[MENUBUTTON_0] = img->copy(0, 0, 9, 9);
		m_items[MENUBUTTON_1] = img->copy(0, 9, 9, 9);
	
		m_items[MINIMIZE_0] = img->copy(9, 0, 9, 9);
		m_items[MINIMIZE_1] = img->copy(9, 9, 9, 9);
	
		m_items[CLOSE_0] = img->copy(18, 0, 9, 9);
		m_items[CLOSE_1] = img->copy(18, 9, 9 ,9);
	
		m_items[SHADE_1_0] = img->copy(0, 18, 9, 9);
		m_items[SHADE_1_1] = img->copy(9, 18, 9, 9);
	
		m_items[SHADE_2_0] = img->copy(0, 27, 9, 9);
		m_items[SHADE_2_1] = img->copy(9, 27, 9, 9);
	
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
		m_items[POSBAR] = img->copy (0, 0, 248, qMin (10, img->height ()));
		m_items[POSBAR_BTN_0] = img->copy (248, 0, 29, qMin (10, img->height ()));
		m_items[POSBAR_BTN_1] = img->copy (278, 0, 29, qMin (10, img->height ()));

		delete img;
	} else {
		setSkin(":CleanAMP/");
	}

	img = getPixmap("volume");
	if (img) {
		for (int i = 0; i <= 27; i++) {
			m_items[VOLUMEBAR_POS_0+i] = img->copy(0, i*15, 68, 13);
		}

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
		for (int i = 0; i < 28; i++) {
			m_items[BALANCE_POS_0+i] = img->copy(9, i*15, 38, 13);
		}

		if (img->height() > 421) {
			m_items[BALANCE_BTN_0] = img->copy(0, 422, 14, qMin (11, img->height () - 422));//11);
			m_items[BALANCE_BTN_1] = img->copy(15, 422, 14, qMin (11, img->height () - 422)); //11);
		}

		delete img;
	} else {
		setSkin(":CleanAMP/");
	}
}


void
Skin::BuildNumbers (void)
{
	uint num = 11;

	QPixmap *img = getPixmap("numbers");
	if (!img) {
		num = 12;
		img = getPixmap ("nums_ex");
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

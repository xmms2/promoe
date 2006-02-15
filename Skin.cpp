#include "Skin.h"


Skin::Skin (string m_skinname) : m_skinname (m_skinname)
{
	m_path = QString ("./CleanAMP/");

	m_items = new QHash<uint, QPixmap>;
	m_volume_bar = new QHash<uint, QPixmap>;
	m_balance = new QHash<uint, QPixmap>;
	
	BuildLetterMap();
	BuildButtons();
	BuildToggleButtons();
	BuildSliders();
	BuildOther();
	BuildTitleBar();
}


Skin::~Skin ()
{
}

QPixmap *Skin::GetPixmap (string file)
{
	/* check for files in zip and check if file exists */

	QDir dir;
	QString f = QString::fromStdString (file);

	dir.setPath (m_path);
	dir.setFilter (QDir::Files);

	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		if (fileInfo.fileName().toLower() == f) {
			return new QPixmap (fileInfo.filePath());
		}
	}

	return NULL;

	/*return new QPixmap ((m_path+file).c_str());*/
}

void Skin::BuildToggleButtons (void)
{
	QPixmap *img = GetPixmap("shufrep.bmp");

	m_items->insert(REPEAT_ON_0, img->copy(0, 30, 28, 15));
	m_items->insert(REPEAT_ON_1, img->copy(0, 45, 28, 15));

	m_items->insert(REPEAT_OFF_0, img->copy(0,  0, 28, 15));
	m_items->insert(REPEAT_OFF_1, img->copy(0, 15, 28, 15));

	m_items->insert(SHUFFLE_ON_0, img->copy(28, 30, 46, 15));
	m_items->insert(SHUFFLE_ON_1, img->copy(28, 45, 46, 15));

	m_items->insert(SHUFFLE_OFF_0, img->copy(28,  0, 46, 15));
	m_items->insert(SHUFFLE_OFF_1, img->copy(28, 15, 46, 15));

	m_items->insert(EQ_ON_0, img->copy( 0, 73, 23, 12));
	m_items->insert(EQ_ON_1, img->copy(46, 73, 23, 12));

	m_items->insert(EQ_OFF_0, img->copy( 0, 61, 23, 12));
	m_items->insert(EQ_OFF_1, img->copy(46, 61, 23, 12));

	m_items->insert(PLS_ON_0, img->copy(23, 73, 23, 12));
	m_items->insert(PLS_ON_1, img->copy(69, 73, 23, 12));

	m_items->insert(PLS_OFF_0, img->copy(23, 61, 23, 12));
	m_items->insert(PLS_OFF_1, img->copy(69, 61, 23, 12));

	delete img;
}

void Skin::BuildSliders (void)
{
	QPixmap *img;
	
	img = GetPixmap("volume.bmp");
	for (int i = VOLUMEBAR_POS_MIN; i < VOLUMEBAR_POS_MAX; i++) {
		m_volume_bar->insert(i, img->copy(0, i*15, 68, 13));
	}
	delete img;
	
	img =  GetPixmap("balance.bmp");
	if (!img) {
		img = GetPixmap("volume.bmp");
	}

	for (int i = BALANCE_POS_MIN; i < BALANCE_POS_MAX; i++) {
		m_balance->insert(i, img->copy(9, i*15, 38, 13));
	}
	delete img;
}

void Skin::BuildTitleBar (void)
{
	QPixmap *img = GetPixmap("titlebar.bmp");

	m_items->insert(MENUBUTTON_0, img->copy(0, 0, 9, 9));
	m_items->insert(MENUBUTTON_1, img->copy(0, 9, 9, 9));

	m_items->insert(MINIMIZE_0, img->copy(9, 0, 9, 9));
	m_items->insert(MINIMIZE_1, img->copy(9, 9, 9, 9));

	m_items->insert(CLOSE_0, img->copy(18, 0, 9, 9));
	m_items->insert(CLOSE_1, img->copy(18, 9, 9 ,9));

	m_items->insert(SHADE_1_0, img->copy(0, 18, 9, 9));
	m_items->insert(SHADE_1_1, img->copy(9, 18, 9, 9));

	m_items->insert(SHADE_2_0, img->copy(0, 27, 9, 9));
	m_items->insert(SHADE_2_1, img->copy(9, 27, 9, 9));

	m_items->insert(TITLEBAR_0, img->copy(27, 0, 275, 14));
	m_items->insert(TITLEBAR_1, img->copy(27, 15, 275, 14));

	m_items->insert(STATUSBAR_0, img->copy(27, 29, 275, 14));
	m_items->insert(STATUSBAR_1, img->copy(27, 29+13, 275, 14));

	delete img;
}


void Skin::BuildButtons (void)
{
	QPixmap *img = GetPixmap("cbuttons.bmp");
	
	m_items->insert (BTN_PREV_0, img->copy(0,  0, 23, 18));
	m_items->insert (BTN_PREV_1, img->copy(0, 18, 23, 18));

	m_items->insert (BTN_PLAY_0, img->copy(23,  0, 23, 18));
	m_items->insert (BTN_PLAY_1, img->copy(23, 18, 23, 18));

	m_items->insert (BTN_PAUSE_0, img->copy(46,  0, 23, 18));
	m_items->insert (BTN_PAUSE_1, img->copy(46, 18, 23, 18));

	m_items->insert (BTN_STOP_0, img->copy(69,  0, 23, 18));
	m_items->insert (BTN_STOP_1, img->copy(69, 18, 23, 18));

	m_items->insert (BTN_NEXT_0, img->copy(92,  0, 22, 18));
	m_items->insert (BTN_NEXT_1, img->copy(92, 18, 22, 18));

	m_items->insert (BTN_EJECT_0, img->copy(114, 0, 22, 16));
	m_items->insert (BTN_EJECT_1, img->copy(114, 16, 22, 16));

	delete img;
}

void Skin::BuildOther (void)
{
	QPixmap *img;
	QPainter(painter);
	
	img = GetPixmap("monoster.bmp");

	m_items->insert (MONO_0, img->copy(29,  0, 27, 12));
	m_items->insert (MONO_1, img->copy(29, 12, 27, 12));
	
	m_items->insert (STEREO_0, img->copy(0,  0, 29, 12));
	m_items->insert (STEREO_1, img->copy(0, 12, 29, 12));

	/*
	img = GetPixmap("playpaus.bmp");

	m_pic_play = new QPixmap(11, 9);
	painter.begin(m_pic_play);
	painter.drawPixmap (0, 0, 3, 9, *img, 36, 0, 3, 9);
	painter.drawPixmap (3, 0, 8, 9, *img,  1, 0, 8, 9);
	painter.end();

	m_pic_pause = new QPixmap(11, 9);
	painter.begin(m_pic_pause);
	painter.drawPixmap (0, 0, 2, 9, *img, 27, 0, 2, 9);
	painter.drawPixmap (2, 0, 9, 9, *img,  9, 0, 9, 9);
	painter.end();

	m_pic_stop = new QPixmap(11, 9);
	painter.begin(m_pic_stop);
	painter.drawPixmap (0, 0, 2, 9, *img, 27, 0, 2, 9);
	painter.drawPixmap (2, 0, 9, 9, *img, 18, 0, 9, 9);
	painter.end();
	*/
	
	img = GetPixmap ("main.bmp");
	m_items->insert (MAIN_WINDOW, img->copy());
	m_items->insert (ABOUT_0, img->copy(247, 83, 20, 25));
	m_items->insert (ABOUT_1, img->copy(247, 83, 20, 24)); 
	delete img;
}


void Skin::BuildLetterMap (void)
{
	QPixmap *img = GetPixmap("text.bmp");

	QList<QList<QPixmap> >(letters);
	for (int i = 0; i < 3; i++) {
		QList<QPixmap>(l);
		for (int j = 0; j < 31; j++) {
			l.append(img->copy(j*5, i*6, 4, 6));
		}
		letters.append(l);
	}

	delete img;

	m_letterMap = new QHash<uint, QPixmap>;

	/* alphabet */
	for (uint i = 97; i < 123; i++) {
		m_letterMap->insert(i, letters[0][i-97]);
	}

	/* digits */
	for (uint i = 0; i <= 9; i++) {
		m_letterMap->insert(i+48, letters[1][i]);
	}

	/* special characters */
	m_letterMap->insert('"',  letters[0][27]);
	m_letterMap->insert('@',  letters[0][28]);
	m_letterMap->insert('(',  letters[1][13]);
	m_letterMap->insert(')',  letters[1][14]);
	m_letterMap->insert('-',  letters[1][15]);
	m_letterMap->insert('\'', letters[1][16]);
	m_letterMap->insert('`',  letters[1][16]);
	m_letterMap->insert('!',  letters[1][17]);
	m_letterMap->insert('_',  letters[1][18]);
	m_letterMap->insert('+',  letters[1][19]);
	m_letterMap->insert('\\', letters[1][20]);
	m_letterMap->insert('/',  letters[1][21]);
	m_letterMap->insert('[',  letters[1][22]);
	m_letterMap->insert(']',  letters[1][23]);
	m_letterMap->insert('^',  letters[1][24]);
	m_letterMap->insert('&',  letters[1][25]);
	m_letterMap->insert('%',  letters[1][26]);
	m_letterMap->insert('.',  letters[1][27]);
	m_letterMap->insert(',',  letters[1][27]);
	m_letterMap->insert('=',  letters[1][28]);
	m_letterMap->insert('$',  letters[1][29]);
	m_letterMap->insert('#',  letters[1][30]);

	m_letterMap->insert(229, letters[2][0]);
	m_letterMap->insert(246, letters[2][1]);
	m_letterMap->insert(228, letters[2][2]);
	m_letterMap->insert('?', letters[2][3]);
	m_letterMap->insert('*', letters[2][4]);
	m_letterMap->insert(' ', letters[2][5]);

	/* text background */
	m_items->insert (TEXTBG, letters[2][6]);
}

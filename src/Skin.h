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

#ifndef __SKIN_H__
#define __SKIN_H__

#include <QObject>
#include <QList>
#include <QMap>

#include <QSize>
#include <QPoint>
#include <QIcon>
#include <QPixmap>

class QDir;

typedef QList <QPixmap> QPixmapList;
typedef QMap <int, QPixmap> PixmapMap;

class Skin : public QObject
{
	Q_OBJECT
	public:
		static Skin *getInstance (void);

		bool setSkin (const QString& name);
		static const QPixmap getPixmap (const QString&, QDir);

		const QSize getSize (uint item) const { return m_sizes[item]; };
		const QPoint getPos (uint item) const { return m_positions[item]; };
		const QIcon getIcon (uint item) const { return m_icons.value(item); };
		const QPixmapList getBackgrounds (uint item) const
		                                 { return m_backgrounds.value(item); };
		const PixmapMap getNumbers () const { return m_numbers; }
		const PixmapMap getSmallNumbers () const { return m_smallNumbers; }

		const QPixmap getItem (uint part) const { return m_items[part]; }
		const QPixmap getPls (uint part) const { return m_playlist[part]; }
		const QPixmap getLetter (uint c) const { return m_letterMap[c]; }
//		const QPixmap getNumber (uint c) const { return m_numbers[c]; }
		const QByteArray getPLeditValue (QByteArray c) const { return m_pledit_txt[c]; }

	signals:
		void skinChanged (Skin *skin);

	private:
		Skin();
		static Skin *singleton;

		void setSizes ();
		void setPositions ();

		const QPixmap getPixmap (const QString& file);
		bool BuildLetterMap (void);
		bool BuildButtons (void);
		bool BuildToggleButtons (void);
		bool BuildTitleBar (void);
		bool BuildSliders (void);
		bool BuildOther (void);
		bool BuildNumbers (void);
		bool BuildPlaylist (void);
		bool ParsePLEdit (void);
		bool BuildEqualizer (void);

		QString m_skinname;
		QString m_path;

		QList<QPoint> m_positions;
		QList<QSize> m_sizes;

		QMap<uint, QIcon> m_icons;
		QMap<uint, QPixmapList> m_backgrounds;

		QMap<int, QPixmap> m_numbers;
		QMap<int, QPixmap> m_smallNumbers;
		QMap<uint, QPixmap> m_items;
		QMap<uint, QPixmap> m_letterMap;
		QMap<uint, QPixmap> m_playlist;

		QMap<QByteArray, QByteArray> m_pledit_txt;

	public:
		enum Part {
			/* Mainwindow buttons */
			BUTTON_MW_MENU,
			BUTTON_MW_MINIMIZE,
			BUTTON_MW_CLOSE,
			BUTTON_MW_SHADE,
			BUTTON_MW_PREV,
			BUTTON_MW_PLAY,
			BUTTON_MW_PAUSE,
			BUTTON_MW_STOP,
			BUTTON_MW_NEXT,
			BUTTON_MW_EJECT,
			BUTTON_MW_EQ,
			BUTTON_MW_PLS,
			BUTTON_MW_SHUFFLE,
			BUTTON_MW_REPEAT,
			/* Mainwindow buttons shaded */
			BUTTON_MW_SHADED_UNSHADE,
			/* Equalizer buttons */
			BUTTON_EQ_CLOSE,
			BUTTON_EQ_SHADE,
			BUTTON_EQ_ACTIVE,
			BUTTON_EQ_AUTO,
			BUTTON_EQ_PRESET,
			/* Equalizer buttons shaded */
			BUTTON_EQ_SHADED_CLOSE,
			BUTTON_EQ_SHADED_UNSHADE,
			/* Playlist buttons */
			BUTTON_PLS_CLOSE,
			BUTTON_PLS_SHADE,
			/* Playlist buttons shaded */
			BUTTON_PLS_SHADED_UNSHADE,
			/* Sliders */
			SLIDER_POSBAR,
			SLIDER_POSBAR_BGS,
			SLIDER_VOLUMEBAR,
			SLIDER_VOLUMEBAR_BGS,
			SLIDER_BALANCEBAR,
			SLIDER_BALANCEBAR_BGS,
			SLIDER_EQUALIZER,
			SLIDER_EQUALIZER_BGS,
//		};
//
//		enum Part { // Old and not yet sorted enum
			NONE,
			MAIN_WINDOW,
			ABOUT_0,
			ABOUT_1,
			MONO_0,
			MONO_1,
			STEREO_0,
			STEREO_1,
			TITLEBAR_0,
			TITLEBAR_1,
			STATUSBAR_0,
			STATUSBAR_1,
			SEEKBAR,
			SEEKBAR_POS_0,
			SEEKBAR_POS_1,
			TEXTBG,
			PIC_PLAY,
			PIC_PAUSE,
			PIC_STOP,
			CLUTTER_ON,
			CLUTTER_OFF,
			CLUTTER_O,
			CLUTTER_A,
			CLUTTER_I,
			CLUTTER_D,
			CLUTTER_V,
			EQ_WIN_BG,
			EQ_WIN_GRAPH_BG,
			EQ_WIN_BAR_BTN_0,
			EQ_WIN_BAR_BTN_1,
			POSBAR,
			POSBAR_BTN_0,
			POSBAR_BTN_1,
			VOLBAR_BTN_0,
			VOLBAR_BTN_1,
			BALANCE_BTN_0,
			BALANCE_BTN_1,
		};

		enum PlaylistParts {
			PLS_CORNER_UL_0,
			PLS_CORNER_UL_1,
			PLS_CORNER_UR_0,
			PLS_CORNER_UR_1,

			PLS_TBAR_0,
			PLS_TBAR_1,

			PLS_TFILL_0,
			PLS_TFILL_1,
			PLS_BFILL_0,
			PLS_BFILL_1 = PLS_BFILL_0,

			PLS_VISMINI_0,
			PLS_VISMINI_1 = PLS_VISMINI_0,

			PLS_LFILL_0,
			PLS_LFILL_1 = PLS_LFILL_0,
			PLS_RFILL_0,
			PLS_RFILL_1 = PLS_RFILL_0,
			PLS_RFILL2_0,
			PLS_RFILL2_1 = PLS_RFILL2_0,
			PLS_RFILL3_0,
			PLS_RFILL3_1 = PLS_RFILL3_0,

			PLS_CLOSE_BTN_0,
			PLS_CLOSE_BTN_1,
			PLS_SHADE_BTN_0,
			PLS_SHADE_BTN_1,
			PLS_MAX_BTN_0,
			PLS_MAX_BTN_1,

			PLS_SCROLL_0,
			PLS_SCROLL_1,
			PLS_WS_LE_0,
			PLS_WS_LE_1 = PLS_WS_LE_0,
			PLS_WS_RE_0,
			PLS_WS_RE_1,
			PLS_WS_MID_0,
			PLS_WS_MID_1 = PLS_WS_MID_0,
			PLS_LCBAR,
			PLS_RCBAR,
			
			PLS_MSC_BTN_0,
			PLS_MSC_BTN_1,

			PLS_ADD,
			PLS_ADD_DEC,
			PLS_ADD_URL_0,
			PLS_ADD_URL_1,
			PLS_ADD_DIR_0,
			PLS_ADD_DIR_1,
			PLS_ADD_FIL_0,
			PLS_ADD_FIL_1,

			PLS_DEL,
			PLS_DEL_DEC,
			PLS_DEL_ALL_0,
			PLS_DEL_ALL_1,
			PLS_DEL_CRP_0,
			PLS_DEL_CRP_1,
			PLS_DEL_FIL_0,
			PLS_DEL_FIL_1,

			PLS_SEL,
			PLS_SEL_DEC,
			PLS_SEL_INV_0,
			PLS_SEL_INV_1,
			PLS_SEL_NIL_0,
			PLS_SEL_NIL_1,
			PLS_SEL_ALL_0,
			PLS_SEL_ALL_1,

			PLS_MSC,
			PLS_MSC_DEC,
			PLS_MSC_SRT_0,
			PLS_MSC_SRT_1,
			PLS_MSC_INF_0,
			PLS_MSC_INF_1,
			PLS_MSC_OPT_0,
			PLS_MSC_OPT_1,

			PLS_LST,
			PLS_LST_DEC,
			PLS_LST_NEW_0,
			PLS_LST_NEW_1,
			PLS_LST_SAV_0,
			PLS_LST_SAV_1,
			PLS_LST_OPN_0,
			PLS_LST_OPN_1
		};
};

#endif

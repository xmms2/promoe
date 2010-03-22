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

#ifndef __SKIN_H__
#define __SKIN_H__

#include <QObject>
#include <QList>
#include <QMap>

#include <QSize>
#include <QPoint>
#include <QIcon>
#include <QPixmap>

#include "buttonpixmaps.h"

class QIODevice;

typedef QList <QPixmap> QPixmapList;
typedef QMap <int, QPixmap> PixmapMap;

class Skin : public QObject
{
	Q_OBJECT
	public:
		Skin (const QString &url);
		Skin (Skin *other, const QString &url = "");

		const bool isValid () const
			{ return m_valid; }

		static const QPixmap getPixmap (const QString&, const QString&);
		static const QPixmap getMainPixmap (const QString& path);

		const QSize getSize (uint item) const
			{ return m_sizes.value (item); }
		const QPoint getPos (uint item) const
			{ return m_positions.value (item); }

		const PixmapMap getNumbers () const
			{ return m_numbers; }
		const PixmapMap getSmallNumbers () const
			{ return m_smallNumbers; }

		const ButtonPixmaps getButton (int button) const
			{ return m_buttons.value (button); }
		const QPixmapList getPixmapList (uint item) const
			{ return m_pixmaplists.value(item); };
		const PixmapMap getPixmapFont () const
			{ return m_letterMap; }

		const QPixmap getItem (uint part) const
			{ return m_items.value (part); }
		const QPixmap getPls (uint part) const
			{ return m_playlist.value (part); }
		const QPixmap getLetter (uint c) const
			{ return m_letterMap.value (c); }
		const QByteArray getPLeditValue (QByteArray c) const
			{ return m_pledit_txt.value(c); }

	private:
		void setSizes ();
		void setPositions ();

		// Methods to handle the files of a skin
		bool handle_main       (const QPixmap &);
		bool handle_titlebar   (const QPixmap &);
		bool handle_posbar     (const QPixmap &);
		bool handle_volume     (const QPixmap &);
		bool handle_balance    (const QPixmap &);
		bool handle_cbuttons   (const QPixmap &);
		bool handle_monoster   (const QPixmap &);
		bool handle_playpaus   (const QPixmap &);
		bool handle_shufrep    (const QPixmap &);
		bool handle_text       (const QPixmap &);
		bool handle_numbers    (const QPixmap &);
		bool handle_eqmain     (const QPixmap &);
		bool handle_eq_ex      (const QPixmap &);
		bool handle_pledit     (const QPixmap &);
		bool handle_pledit_txt (QIODevice *);

		bool setSkin (const QString& name);

		bool m_valid;
		QString m_skinname;
		QString m_path;

		QMap<int, ButtonPixmaps> m_buttons;
		QMap<uint, QPixmapList> m_pixmaplists;

		QMap<int, QPixmap> m_numbers;
		QMap<int, QPixmap> m_smallNumbers;
		QMap<uint, QPixmap> m_items;
		QMap<int, QPixmap> m_letterMap;
		QMap<uint, QPixmap> m_playlist;

		QMap<QByteArray, QByteArray> m_pledit_txt;

		/* Initialized on class construction */
		QMap<uint, QPoint> m_positions;
		QMap<uint, QSize> m_sizes;

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
			BUTTON_MW_SHADED_MENU,
			BUTTON_MW_SHADED_MINIMIZE,
			BUTTON_MW_SHADED_CLOSE,
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
			BUTTON_PLS_SHADED_CLOSE,
			BUTTON_PLS_SHADED_UNSHADE,
			/* Sliders */
			SLIDER_POSBAR,
			SLIDER_POSBAR_BGS,
			SLIDER_POSBAR_BUTTON,
			SLIDER_VOLUMEBAR,
			SLIDER_VOLUMEBAR_BGS,
			SLIDER_VOLUMEBAR_BUTTON,
			SLIDER_BALANCEBAR,
			SLIDER_BALANCEBAR_BGS,
			SLIDER_BALANCEBAR_BUTTON,
			SLIDER_EQUALIZER,
			SLIDER_EQUALIZER_BGS,
			/* Various */
			PIXMAPS_CLUTTERBAR,
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
			TEXTBG,
			PIC_PLAY,
			PIC_PAUSE,
			PIC_STOP,
			EQ_WIN_BG,
			EQ_WIN_TITLE_ACTIVE,
			EQ_WIN_TITLE_INACTIVE,
			EQ_WIN_GRAPH_BG,
			EQ_WIN_BAR_BTN_0,
			EQ_WIN_BAR_BTN_1,
			EQ_WIN_SHADE_ACTIVE,
			EQ_WIN_SHADE_INACTIVE,
			POSBAR,
		};

		// The order here must be the same as in handle_titlebar
		enum ClutterItems {
			CLUTTER_ON,
			CLUTTER_OFF,
			CLUTTER_O,
			CLUTTER_A,
			CLUTTER_I,
			CLUTTER_D,
			CLUTTER_V
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

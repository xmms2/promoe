#ifndef __SKIN_H__
#define __SKIN_H__

#include <iostream>

#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QHash>
#include <QDir>

using namespace std;

class Skin : public QWidget
{
	Q_OBJECT
	public:
		Skin ();
		~Skin();

		void setSkin (QString name);
		
		const QPixmap getItem (uint part) const { return m_items->value(part); }
		const QPixmap getVol (uint p) const { return m_volume_bar->value(p); }
		const QPixmap getBal (uint p) const { return m_balance->value(p); }
		const QPixmap getLetter (uint c) const { return m_letterMap->value(c); }
		const QPixmap getNumber (uint c) const { return m_numbers->value(c); }
		const QByteArray getPLeditValue (QByteArray c) const { return m_pledit_txt->value(c); }

		enum Volume {
			VOLUMEBAR_POS_MIN,
			VOLUMEBAR_POS_1,
			VOLUMEBAR_POS_2,
			VOLUMEBAR_POS_3,
			VOLUMEBAR_POS_4,
			VOLUMEBAR_POS_5,
			VOLUMEBAR_POS_6,
			VOLUMEBAR_POS_7,
			VOLUMEBAR_POS_8,
			VOLUMEBAR_POS_9,
			VOLUMEBAR_POS_10,
			VOLUMEBAR_POS_11,
			VOLUMEBAR_POS_12,
			VOLUMEBAR_POS_13,
			VOLUMEBAR_POS_14,
			VOLUMEBAR_POS_15,
			VOLUMEBAR_POS_16,
			VOLUMEBAR_POS_17,
			VOLUMEBAR_POS_18,
			VOLUMEBAR_POS_19,
			VOLUMEBAR_POS_20,
			VOLUMEBAR_POS_21,
			VOLUMEBAR_POS_22,
			VOLUMEBAR_POS_23,
			VOLUMEBAR_POS_24,
			VOLUMEBAR_POS_25,
			VOLUMEBAR_POS_26,
			VOLUMEBAR_POS_MAX
		};

		enum Balance {
			BALANCE_POS_MIN,
			BALANCE_POS_1,
			BALANCE_POS_2,
			BALANCE_POS_3,
			BALANCE_POS_4,
			BALANCE_POS_5,
			BALANCE_POS_6,
			BALANCE_POS_7,
			BALANCE_POS_8,
			BALANCE_POS_9,
			BALANCE_POS_10,
			BALANCE_POS_11,
			BALANCE_POS_12,
			BALANCE_POS_13,
			BALANCE_POS_14,
			BALANCE_POS_15,
			BALANCE_POS_16,
			BALANCE_POS_17,
			BALANCE_POS_18,
			BALANCE_POS_19,
			BALANCE_POS_20,
			BALANCE_POS_21,
			BALANCE_POS_22,
			BALANCE_POS_23,
			BALANCE_POS_24,
			BALANCE_POS_25,
			BALANCE_POS_26,
			BALANCE_POS_MAX
		};

		enum Part {
			MAIN_WINDOW,
			ABOUT_0,
			ABOUT_1,
			MONO_0,
			MONO_1,
			STEREO_0,
			STEREO_1,
			MENUBUTTON_0,
			MENUBUTTON_1,
			MINIMIZE_0,
			MINIMIZE_1,
			CLOSE_0,
			CLOSE_1,
			SHADE_1_0,
			SHADE_1_1,
			SHADE_2_0,
			SHADE_2_1,
			TITLEBAR_0,
			TITLEBAR_1,
			STATUSBAR_0,
			STATUSBAR_1,
			SEEKBAR,
			SEEKBAR_POS_0,
			SEEKBAR_POS_1,
			REPEAT_ON_0,
			REPEAT_ON_1,
			REPEAT_OFF_0,
			REPEAT_OFF_1,
			SHUFFLE_ON_0,
			SHUFFLE_ON_1,
			SHUFFLE_OFF_0,
			SHUFFLE_OFF_1,
			EQ_ON_0,
			EQ_ON_1,
			EQ_OFF_0,
			EQ_OFF_1,
			PLS_ON_0,
			PLS_ON_1,
			PLS_OFF_0,
			PLS_OFF_1,
			BTN_PREV_0,
			BTN_PREV_1,
			BTN_PLAY_0,
			BTN_PLAY_1,
			BTN_PAUSE_0,
			BTN_PAUSE_1,
			BTN_STOP_0,
			BTN_STOP_1,
			BTN_NEXT_0,
			BTN_NEXT_1,
			BTN_EJECT_0,
			BTN_EJECT_1,
			TEXTBG,
			PIC_PLAY,
			PIC_PAUSE,
			PIC_STOP,
			POSBAR,
			POSBAR_BTN_0,
			POSBAR_BTN_1,
		};
	private:
		QPixmap *Skin::getPixmap (string file);
		void Parse (string file);
		void BuildLetterMap (void);
		void BuildButtons (void);
		void BuildToggleButtons (void);
		void BuildTitleBar (void);
		void BuildSliders (void);
		void BuildOther (void);
		void BuildNumbers (void);
		void ParsePLEdit (void);


		string m_skinname;
		QString m_path;

		QHash<uint, QPixmap> *m_items;
		QHash<uint, QPixmap> *m_letterMap;
		QHash<uint, QPixmap> *m_volume_bar;
		QHash<uint, QPixmap> *m_balance;
		QHash<uint, QPixmap> *m_numbers;

		QHash<QByteArray, QByteArray> *m_pledit_txt;
		QList<QPixmap *> m_buttons;

	signals:
		void skinChanged (Skin *skin);
};

#endif

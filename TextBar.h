#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

#include <iostream>

#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QFont>
#include <QTimer>
#include <QBrush>
#include <QPalette>
#include <QBitmap>

#include "Skin.h"

class TextScroller : public QWidget
{
	Q_OBJECT

	public:
		TextScroller (QWidget *parent, uint, uint, const QString &);
		~TextScroller ();

		void setText(QString text);
		void setFontSize (int i) { m_fontsize = i; }
		void setTTF (bool b) { m_ttf = b; }

	public slots:
		void addOffset ();
		void setPixmaps(Skin *skin);
		void settingsSaved (void);

	protected:
		QPixmap m_pixmap;
		void paintEvent (QPaintEvent *event);

	private:
		int m_w;
		int m_h;
		int m_y;
		int m_x_off;
		int m_x2_off;
		int m_fontsize;
		bool m_ttf;
		bool m_scroll;

		QTimer *m_timer;
		QString m_text;
		QString m_name;

		void drawBitmapFont (QString text);
		void drawQtFont (QString text);
};

#endif

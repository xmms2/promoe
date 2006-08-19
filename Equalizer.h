#ifndef __EQUALIZER_H__
#define __EQUALIZER_H__

class Skin;
class Button;
class MainWindow;
class TitleBar;
class ToggleButton;
class Slider;

#include <iostream>

#include <QPixmap>
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMainWindow>

#include "Display.h"

class EqualizerWidget : public QWidget
{
	Q_OBJECT
	public:
		EqualizerWidget(QWidget *parent);
		~EqualizerWidget();
		void mouseMoveEvent(QMouseEvent *);
		void paintEvent (QPaintEvent *event);

	public slots:
		void setPixmaps(Skin *skin);
	private:
		QPixmap m_pixmap;
		QPixmap m_graph;
		ToggleButton *m_enable;
		ToggleButton *m_auto;
		Button *m_preset;
		Slider *m_preamp;
		Slider *m_bands[10];
};

class EqualizerWindow : public QMainWindow
{
	Q_OBJECT
	public:
		EqualizerWindow(QWidget *parent);
		~EqualizerWindow();

	public slots:
		void setEnabled (void);

	private:
		MainWindow *m_mw;
		EqualizerWidget *m_equalizer;
};


#endif

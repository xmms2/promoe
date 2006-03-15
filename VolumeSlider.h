#ifndef __VOLUMESLIDER_H__
#define __VOLUMESLIDER_H__

#include "PixWidget.h"
#include "Button.h"

class VolumeSlider;
class MainDisplay;

class VolButton : public PixWidget
{
	Q_OBJECT
	public:
		VolButton (QWidget *, uint, uint);
		~VolButton ();

		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);
		void mouseReleaseEvent (QMouseEvent *);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		void changePixmap (bool pressed);
		void drawPixmaps();

		VolumeSlider *m_volslider;
		Skin *m_skin;
		QPixmap m_volbtn;
		uint m_normal;
		uint m_pressed;
};

class VolumeSlider : public PixWidget
{
	Q_OBJECT
	public:
		VolumeSlider (QWidget *parent);
		~VolumeSlider ();
		void mouseMoveEvent (QMouseEvent *);
		void mousePressEvent (QMouseEvent *);
		void mouseReleaseEvent (QMouseEvent *);

		uint getVolume (void) { return m_volume_base100; };
		void setVolume (uint volume_base100);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		void drawPixmaps ();
		void updatePos (QMouseEvent *event);
		void changePixmap ();

		QPixmap m_volslider;
		VolButton *m_volbtn;
		Skin *m_skin;
		int m_volume;
		int m_position;
		uint m_volume_base100;
		bool m_hasvolbtn;
		
};

#endif

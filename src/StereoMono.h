#ifndef __STEREOMONO_H__
#define __STEREOMONO_H__

#include "PixWidget.h"

class StereoMono : public PixWidget
{
	public:
		StereoMono (QWidget *parent);
		~StereoMono () { }

		void setStereoMono (bool, bool);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		void drawPixmaps ();
		bool m_stereo;
		bool m_mono;

		QPixmap m_pixmap_stereo_on;
		QPixmap m_pixmap_stereo_off;
		QPixmap m_pixmap_mono_on;
		QPixmap m_pixmap_mono_off;

		QPixmap m_pixmap_mono;
		QPixmap m_pixmap_stereo;
};

#endif

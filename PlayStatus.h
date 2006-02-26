#ifndef __PLAYSTATUS_H__
#define __PLAYSTATUS_H__

#include "XMMSHandler.h"
#include "PixWidget.h"

class PlayStatus : public PixWidget
{
	Q_OBJECT
	public:
		PlayStatus (QWidget *parent);
		~PlayStatus () { }


	public slots:
		void setPixmaps (Skin *skin);
		void setStatus (uint status);

	private:
		int m_status;

		QPixmap m_pixmap_stop;
		QPixmap m_pixmap_play;
		QPixmap m_pixmap_pause;
};

#endif

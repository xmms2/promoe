#ifndef __PLAYSTATUS_H__
#define __PLAYSTATUS_H__

#include <xmmsclient/xmmsclient.h>
#include "PixWidget.h"

class PlayStatus : public PixWidget
{
	public:
		PlayStatus (QWidget *parent);
		~PlayStatus () { }

		void setStatus (int);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		int m_status;

		QPixmap m_pixmap_stop;
		QPixmap m_pixmap_play;
		QPixmap m_pixmap_pause;

};

#endif

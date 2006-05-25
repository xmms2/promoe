#ifndef __PLAYSTATUS_H__
#define __PLAYSTATUS_H__

#include <xmmsclient/xmmsclient++.h>
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
		void setStatus (Xmms::Playback::Status status);

	private:
		Xmms::Playback::Status m_status;

		QPixmap m_pixmap_stop;
		QPixmap m_pixmap_play;
		QPixmap m_pixmap_pause;
};

#endif

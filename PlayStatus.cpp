#include "PlayStatus.h"

PlayStatus::PlayStatus (QWidget *parent) : PixWidget (parent)
{
		setMinimumSize(11, 9);
		setMaximumSize(11, 9);

		m_status = XMMS_PLAYBACK_STATUS_STOP;
}

void
PlayStatus::setPixmaps (Skin *skin)
{
	m_pixmap_play = skin->getItem (Skin::PIC_PLAY);
	m_pixmap_pause = skin->getItem (Skin::PIC_PAUSE);
	m_pixmap_stop = skin->getItem (Skin::PIC_STOP);

	setStatus (m_status);
}

void
PlayStatus::setStatus (int status)
{

	if (status == XMMS_PLAYBACK_STATUS_STOP) {
		m_pixmap = m_pixmap_stop;
	} else if (status == XMMS_PLAYBACK_STATUS_PLAY) {
		m_pixmap = m_pixmap_play;
	} else if (status == XMMS_PLAYBACK_STATUS_PAUSE) {
		m_pixmap = m_pixmap_pause;
	}

	update ();
}


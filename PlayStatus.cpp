#include "PlayStatus.h"
#include "Skin.h"

PlayStatus::PlayStatus (QWidget *parent) : PixWidget (parent)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance();

	setMinimumSize(11, 9);
	setMaximumSize(11, 9);

	m_status = XMMS_PLAYBACK_STATUS_STOP;

	connect (xmmsh, SIGNAL(playbackStatusChanged(uint)),
	         this, SLOT(setStatus(uint)));
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
PlayStatus::setStatus (uint status)
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


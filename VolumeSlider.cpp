#include "XMMSHandler.h"

#include "VolumeSlider.h"
#include <QMouseEvent>

// NOTE!
// This file has lots of seemingly strange math.
// I will document it as we go, but please keep in mind
// that all of the coord-space and normalization is pixel-space dependent.

VolumeSlider::VolumeSlider (QWidget *parent) : PixWidget (parent)
{
	XMMSHandler *xmmsh = XMMSHandler::getInstance ();

	setMinimumSize (68, 13);
	setMaximumSize (68, 13);
	m_volume = 0;
	m_position = 0;
	m_hasvolbtn = false;
	m_volbtn = NULL;

	m_pixmap = QPixmap (68, 13);

	connect (xmmsh, SIGNAL(getVolume (uint)),
		 this, SLOT(setVolume (uint)));

	xmmsh->volumeGet ();
}

VolumeSlider::~VolumeSlider ()
{

}

void
VolumeSlider::setPixmaps (Skin *skin)
{
	m_skin = skin;

	if(m_skin->getVolBtn())
	{
		m_hasvolbtn = true;
		if(m_volbtn)
			delete m_volbtn;

		m_volbtn = new VolButton (this, Skin::VOLBAR_BTN_0, Skin::VOLBAR_BTN_1);

		// If we are out of scope high or low, we clamp the values.
		if(m_volume <= 0)
			m_volbtn->move (0, 1);
		else if(m_volume >= 27)
			m_volbtn->move (54, 1);
		else
		{
			// If we are not.. we do two things.

			// First, we normalize the difference between the current position
			// and the maximum position. This will give us the progress ratio.
			float temp = ((float)(m_volume) / (float)(27.0f));

			// Now, we multiply that by the number of possible positions in our
			// space. This gives us our relative position according to the progress
			// ratio.
			m_volbtn->move ((int)(49 * temp), 1);
		}
		m_volbtn->setPixmaps (m_skin);
		m_volbtn->show ();
	}
	else
	{
		m_hasvolbtn = false;
		if(m_volbtn)
		{
			delete m_volbtn;
			m_volbtn = NULL;
		}
	}

	m_volslider = m_skin->getVol (m_volume);

	drawPixmaps ();
}

void
VolumeSlider::changePixmap ()
{
	m_volslider = m_skin->getVol (m_volume);
	drawPixmaps ();
}

void
VolumeSlider::drawPixmaps ()
{
	QPainter paint;
	paint.begin (&m_pixmap);

	paint.drawPixmap (QRect (0, 0, 68, 13),
					  m_volslider,
					  m_pixmap.rect ());
	paint.end ();

	update ();
}

void 
VolumeSlider::mousePressEvent (QMouseEvent *event)
{
	updatePos (event);
	if(m_hasvolbtn)
	{
		m_volbtn->mousePressEvent (event);
	}
}

void
VolumeSlider::mouseMoveEvent (QMouseEvent *event)
{
	updatePos (event);
	if(m_hasvolbtn)
	{
		m_volbtn->mouseMoveEvent (event);
	}
}

void
VolumeSlider::mouseReleaseEvent (QMouseEvent *event)
{
	updatePos (event);
	if(m_hasvolbtn)
	{
		m_volbtn->mouseReleaseEvent (event);
	}
}

void
VolumeSlider::updatePos (QMouseEvent *event)
{
	QPoint p (event->pos ());

	int curx = p.x ();

	// Check for scope and clamp.
	if(curx <= 0)
	{
		m_volume = 0;
		m_volume_base100 = 0;
	}
	else if(curx >= 68)
	{
		m_volume = 27;
		m_volume_base100 = 100;
	}
	else
	{
		// Normalize the same way we do above, except this time
		// we calculate a base-100 value as well.
		float temp = ((float)(curx - 5) / (float)(width() - 5));
		m_volume_base100 = (int)(100 * temp);
		m_volume = (int)(28 * temp);
	}

	changePixmap ();
}

void
VolumeSlider::setVolume (uint volume_base100)
{
	m_volume_base100 = volume_base100;
	if(volume_base100 > 100)
		volume_base100 = 100;

	m_volume = (int)((float)(volume_base100) *.28);
	if(m_volume > 27)
		m_volume = 27;

	if(m_hasvolbtn)
		m_volbtn->setVolume (volume_base100);

	changePixmap ();
}

VolButton::VolButton (QWidget *parent, uint normal, uint pressed) : PixWidget (parent)
{
	m_volslider = dynamic_cast<VolumeSlider *>(parent);
	setMinimumSize (14, 11);
	setMaximumSize (14, 11);

	m_normal = normal;
	m_pressed = pressed;

	m_pixmap = QPixmap (14, 11);
}

VolButton::~VolButton ()
{

}

void
VolButton::mousePressEvent (QMouseEvent *event)
{
	QPoint p (event->globalPos ());
	QPoint np = m_volslider->mapFromGlobal (p);

	move (np.x() - 7, 1);

	changePixmap (true);
}

void
VolButton::mouseReleaseEvent (QMouseEvent *event)
{
	changePixmap (false);
}

void
VolButton::mouseMoveEvent (QMouseEvent *event)
{
	QPoint p = m_volslider->mapFromGlobal (event->globalPos ());
	int volume = 0;
	int curx = p.x ();

	// Same deal, clamp then normalize.
	if(curx < 7)
	{
		volume = 0;
		m_volslider->setVolume (0);
	}
	else if(curx > 61)
	{
		volume = 54;
		m_volslider->setVolume (100);
	}
	else
	{
		float temp = ((float)(((float)curx) / (float)(68.0f)));
		float b100temp = ((float)(((float)curx - 12) / (float)(68.0f - 19)));
		volume = (int)(68 * temp) - 7;
		// This is to make sure the volume slider itself reflects our changes.
		m_volslider->setVolume ((int)(100 * b100temp));
	}

	move (volume, 1);
}

void
VolButton::setVolume (uint volume_base100)
{
	int volume = (int)((float)(volume_base100) *.68);

	if(volume < 0)
		volume = 0;
	else if(volume > 54)
		volume = 54;

	move (volume,1);
}


void
VolButton::setPixmaps (Skin *skin)
{
	m_skin = skin;

	m_volbtn = m_skin->getItem (m_normal);

	drawPixmaps ();
}

void
VolButton::changePixmap (bool pressed)
{
	if(pressed)
		m_volbtn = m_skin->getItem (m_pressed);
	else
		m_volbtn = m_skin->getItem (m_normal);

	drawPixmaps ();
}

void
VolButton::drawPixmaps ()
{
	QPainter paint;
	paint.begin (&m_pixmap);

	paint.drawPixmap (QRect (0, 0, 14, 11),
					  m_volbtn,
					  m_pixmap.rect ());
	paint.end ();

	update ();
}

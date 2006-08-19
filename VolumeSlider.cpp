#include "XMMSHandler.h"
#include <math.h>

#include "VolumeSlider.h"
#include <QMouseEvent>

#include "Skin.h"
#include "Button.h"

#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

Slider::Slider (QWidget *parent, uint name_min, uint name_max,
                uint name_on, uint name_off, int min, int max) : PixWidget (parent)
{
	m_name_min = name_min;
	m_name_max = name_max;
	m_name_on = name_on;
	m_name_off = name_off;
	
	m_min = min;
	m_max = max;

	m_button = 0;

	m_value = 0;
	m_value_index = (uint)((abs(m_min) / (double)(abs(m_min)+abs(m_max))) * (name_max-name_min));
}

void
Slider::setPixmaps (Skin *skin)
{
	m_skin = skin;

	m_pixmap_slider = m_skin->getItem(m_name_min+m_value_index);
	m_vertical = (m_pixmap_slider.height() > m_pixmap_slider.width()) ? true : false;

	setMinimumSize(m_pixmap_slider.size());
	setMaximumSize(m_pixmap_slider.size());

	resize(m_pixmap_slider.size());

	if (!skin->getItem(m_name_on).isNull() && !skin->getItem(m_name_off).isNull()) {
		if (m_button) {
			delete m_button;
		}

		m_button = new SliderButton (this, m_name_on, m_name_off, m_vertical);
		m_button->setPixmaps (m_skin);
		m_button->show ();

		int tmp = (uint)((ceil(abs(m_min) / (double)(abs(m_min)+abs(m_max)))) * (m_name_max-m_name_min));
		if (m_vertical) {
			m_button->move(1, height()-m_button->height()-tmp);
		} else {
			m_button->move(tmp, 1);
		}
	} else {
		if (m_button) {
			delete m_button;
			m_button = NULL;
		}
	}

	update();
}

void
Slider::changePixmap ()
{
	m_pixmap_slider = m_skin->getItem (m_name_min+m_value_index);
	update();
}

void
Slider::paintEvent (QPaintEvent *event)
{
	QPainter paint;
	paint.begin (this);
	paint.drawPixmap (rect(), m_pixmap_slider, m_pixmap_slider.rect ());
	paint.end ();
}

void 
Slider::mousePressEvent (QMouseEvent *event)
{
	updatePos (event);
}

void
Slider::mouseMoveEvent (QMouseEvent *event)
{
	updatePos (event);
}

void
Slider::mouseReleaseEvent (QMouseEvent *event)
{
	updatePos (event);
}

void
Slider::updatePos (QMouseEvent *event)
{
	QPoint p (event->pos ());
	int value;

	if (m_vertical) {
		value = CLAMP(p.y(), 0, height());
	} else {
		value = CLAMP(p.x(), 0, width());
	}

	setPos(value, true);
	if (m_button != 0) {
		m_button->setPos(value);
	}

	emit valueChanged (m_value);
}

void
Slider::setValue (int value)
{
	double frac;
	int pos;

   	frac = abs(value)/(double)(abs(m_min)+abs(m_max));
	if (m_vertical) {
		pos = (int)(height() * frac);
	} else {
		pos = (int)(width() * frac);
	}

	setPos(pos, false);
	if (m_button != 0) {
		m_button->setPos(pos);
	}
}

void
Slider::setPos (int value, bool tell)
{
	double frac;

	if (m_vertical) {
		frac = (height() - value) / (double) height();
	} else {
		frac = value / (double) width();
	}

	/* calculate m_min <= m_value <= m_max */
	m_value = (uint)(frac * (abs(m_min) + abs(m_max)) + m_min);

	/* calculate m_name_min <= m_value_index <= m_name_max */
	m_value_index = (uint) ceil (frac * (m_name_max - m_name_min));


	changePixmap();

	if (tell) {
		emit valueChanged (m_value);
	}
}


SliderButton::SliderButton (QWidget *parent, uint normal, uint pressed,
                            bool vertical) : Button (parent, normal, pressed, false)
{
	m_slider = dynamic_cast<Slider *>(parent);
	m_vertical = vertical;
	m_diff = 0;
	m_moving = false;

	if (m_vertical) {
		move(1, 0);
	} else {
		move(0, 1);
	}
}


void
SliderButton::mousePressEvent (QMouseEvent *event)
{
	if (m_vertical) {
		m_diff = y() - event->pos().y();
	} else {
		m_diff = x() - event->pos().x();
	}
	m_moving = true;
}


void
SliderButton::mouseReleaseEvent (QMouseEvent *event)
{
	m_moving = false;
}


void
SliderButton::setPos (uint pos)
{
	if (m_vertical) {
		int ypos = MIN(pos, (uint)m_slider->height()-height());
		move(1, ypos);
	} else {
		int xpos = MIN(pos, (uint)m_slider->width()-width());
		move(xpos, 1);
	}
}


void
SliderButton::mouseMoveEvent (QMouseEvent *event)
{
	QPoint p = m_slider->mapFromGlobal (event->globalPos ());
	int value;
	if (m_vertical) {
		value = CLAMP(p.y(), 0, m_slider->height());
	} else {
		value = CLAMP(p.x(), 0, m_slider->width());
	}

	m_slider->setPos(value, true);
	setPos (value);
}

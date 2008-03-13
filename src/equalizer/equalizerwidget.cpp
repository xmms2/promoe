/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2008 XMMS2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "XMMSHandler.h"
#include "xconfig.h"

#include "equalizerwidget.h"

#include "mainwindow.h"
#include "Button.h"
#include "Skin.h"
#include "VolumeSlider.h"

#include <QPainter>

EqualizerSlider::EqualizerSlider (QWidget *parent, uint pix_min, uint pix_max,
                                  uint pix_on, uint pix_off, int min, int max, 
                                  int id) :
                                  Slider (parent, pix_min, pix_max, pix_on,
                                          pix_off, min, max)
{
	m_id = id;
	connect ( this, SIGNAL (sliderMoved (int)),
              this, SLOT (on_self_slider_moved (int)) );
	setInvertedAppearance (true);
	setOrientation (Qt::Vertical);
}

void
EqualizerSlider::on_self_slider_moved (int value)
{
	emit numberedSliderMoved (value, m_id);
}


/*
 * EqualizerWidget
 */

EqualizerWidget::EqualizerWidget (QWidget *parent) : QWidget (parent)
{
	Skin *skin = Skin::getInstance ();
	XMMSHandler &client = XMMSHandler::getInstance ();
	m_xconfig = client.xconfig ();

	connect (skin, SIGNAL(skinChanged(Skin *)),
	         this, SLOT(setPixmaps(Skin *)));

	m_enable = new ToggleButton(this, Skin::EQ_WIN_ON_0, Skin::EQ_WIN_ON_1,
	                            Skin::EQ_WIN_OFF_0, Skin::EQ_WIN_OFF_1);
	m_enable->move(14, 18);
	// must use signal clicked here, as this button also becomes unchecked if
	// use_legacy is deactivated
	connect(m_enable, SIGNAL (clicked (bool)),
            this, SLOT (setEqualizerEnabled (bool)));


	m_auto = new ToggleButton(this, Skin::EQ_WIN_AUTO_ON_0, Skin::EQ_WIN_AUTO_ON_1,
	                          Skin::EQ_WIN_AUTO_OFF_0, Skin::EQ_WIN_AUTO_OFF_1);
	m_auto->move(39, 18);
	m_auto->setEnabled(false); // FIXME: needs to be implemented

	connect(m_auto, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_preset = new Button(this, Skin::EQ_WIN_PRESET_0, Skin::EQ_WIN_PRESET_1);
	m_preset->move(217, 18);
	m_preset->setEnabled(false); // FIXME: needs to be implemented

	connect(m_preset, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_preamp = new Slider(this, Skin::EQ_WIN_BAR_POS_0, Skin::EQ_WIN_BAR_POS_27,
	                      Skin::EQ_WIN_BAR_BTN_0, Skin::EQ_WIN_BAR_BTN_1,
	                      -20, 20);
	m_preamp->move(21, 38);
	m_preamp->setOrientation (Qt::Vertical);
	m_preamp->setInvertedAppearance (true);
	connect (m_preamp, SIGNAL (sliderMoved (int)),
	         this, SLOT (updateServerPreamp (int)));

	for (int i=0; i < 10; i++) {
		m_bands[i] = new EqualizerSlider(this, Skin::EQ_WIN_BAR_POS_0,
                                         Skin::EQ_WIN_BAR_POS_27,
                                         Skin::EQ_WIN_BAR_BTN_0,
                                         Skin::EQ_WIN_BAR_BTN_1, -20, 20, i);
		m_bands[i]->move(78+i*18, 38);
		connect (m_bands[i], SIGNAL (numberedSliderMoved (int, int)),
		         this, SLOT (updateServerBands (int, int)));
	}

	connect (m_xconfig, SIGNAL (configChanged (QString, QString)),
	         this, SLOT (serverConfigChanged (QString, QString)));

	// if the config from the server were already loaded, we will only
	// receive configChanged signals for values that really change
	// so we must request the existing values manually
	if (m_xconfig->isReady()) {
		QString key;
		QString value;
		// set enabled checkbox
		key = QString ("equalizer.enabled");
		value = m_xconfig->value_get (key);
		serverConfigChanged (key, value);
		// set preamp
		key = QString ("equalizer.preamp");
		value = m_xconfig->value_get (key);
		serverConfigChanged (key, value);
		// Set band-sliders
		for (int i=0; i < 10; i++) {
			key = QString ("equalizer.legacy%1").arg(i);
			value = m_xconfig->value_get (key);
			serverConfigChanged (key, value);
		}
	}
}

EqualizerWidget::~EqualizerWidget (void)
{
}

void
EqualizerWidget::setPixmaps (Skin *skin)
{
	m_pixmap = skin->getItem(Skin::EQ_WIN_BG);
	m_graph = skin->getItem(Skin::EQ_WIN_GRAPH_BG);

	setMinimumSize (m_pixmap.size ());
	setMaximumSize (m_pixmap.size ());

	update();
}

void
EqualizerWidget::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull ()) {
		return;
	}
	QPainter(paint);
	QRect r;

	paint.begin(this);

	paint.drawPixmap(rect(), m_pixmap, m_pixmap.rect());
	r.setRect(86, 17, 113, 19);
	paint.drawPixmap(r, m_graph, m_graph.rect());

	paint.end();
}

/*
 *  These methods handle server configuration updates and
 *  update the serverconfiguraten if we change something
 */
void
EqualizerWidget::serverConfigChanged (QString key, QString value)
{
//	qDebug (key.toAscii ());
//	qDebug (value.toAscii ());

	// FIXME: also test on use_legacy
	if (key.startsWith ("equalizer.enabled")) {
		if (value != "0") {
			m_enable->setChecked (true);
		} else {
			m_enable->setChecked (false);
		}
	}
	if (key == "equalizer.preamp") {
		// FIXME: value can be of type floas
		m_preamp->setValue (value.toInt ());
	}
	if (key.startsWith ("equalizer.legacy")) {
		int i = key.right (1).toInt ();
		// FIXME: value can be float
		m_bands[i]->setValue (value.toInt ());
	}
}

void
EqualizerWidget::setEqualizerEnabled (bool enabled) {
	if (enabled) {
		m_xconfig->value_set ("equalizer.enabled", "1");
		m_xconfig->value_set ("equalizer.use_legacy", "1");
	} else {
		m_xconfig->value_set ("equalizer.enabled", "0");
	}
}

void
EqualizerWidget::updateServerPreamp (int value)
{
	m_xconfig->value_set ("equalizer.preamp", QString::number (value));
}

void
EqualizerWidget::updateServerBands (int value, int id)
{
	QString key = QString ("equalizer.legacy%1").arg (id);
	m_xconfig->value_set (key, QString::number (value));
}

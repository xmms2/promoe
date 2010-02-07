/**
 *  This file is a part of Promoe, an XMMS2 Client.
 *
 *  Copyright (C) 2005-2010 XMMS2 Team
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

#include "xclient.h"
#include "xconfig.h"

#include "equalizerwidget.h"

#include "application.h"
#include "mainwindow.h"
#include "pixmapbutton.h"
#include "pixmapslider.h"
#include "skin.h"
#include "skinmanager.h"


#include <QPainter>
#include <QMessageBox>

EqualizerSlider::EqualizerSlider (QWidget *parent, int id) :
                                  PixmapSlider (parent)
{
	m_id = id;
	connect ( this, SIGNAL (sliderMoved (int)),
              this, SLOT (on_self_slider_moved (int)) );
	setMinimum (-20);
	setMaximum (20);
	setSliderOffset (QPoint (1, 0));
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
	Skin *skin = SkinManager::instance ()->activeSkin ();
	const XClient *client = App->client ();
	m_xconfig = client->xconfig ();

	connect (SkinManager::instance (), SIGNAL(skinChanged(Skin *)),
	         this, SLOT(setPixmaps(Skin *)));

	m_closebtn = new PixmapButton (this);
	m_closebtn->resize (skin->getSize (Skin::BUTTON_EQ_CLOSE));
	m_closebtn->move (skin->getPos (Skin::BUTTON_EQ_CLOSE));
	connect (m_closebtn, SIGNAL (clicked ()), parent, SLOT (hide ()));

	// TODO implement shade mode
	m_shadebtn = new PixmapButton (this);
	m_shadebtn->resize (skin->getSize (Skin::BUTTON_EQ_SHADE));
	m_shadebtn->move (skin->getPos (Skin::BUTTON_EQ_SHADE));
	m_shadebtn->setEnabled (false);

	m_enable = new PixmapButton (this);
	m_enable->setCheckable (true);
	m_enable->resize (skin->getSize (Skin::BUTTON_EQ_ACTIVE));
	m_enable->move (skin->getPos (Skin::BUTTON_EQ_ACTIVE));
	// must use signal clicked here, as this button also becomes unchecked if
	// use_legacy is deactivated
	connect (m_enable, SIGNAL (clicked (bool)),
            this, SLOT (setEqualizerEnabled (bool)));


	m_auto = new PixmapButton (this);
	m_auto->setCheckable (true);
	m_auto->resize (skin->getSize (Skin::BUTTON_EQ_AUTO));
	m_auto->move (skin->getPos (Skin::BUTTON_EQ_AUTO));
	m_auto->setEnabled(false); // FIXME: needs to be implemented

	connect (m_auto, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_preset = new PixmapButton (this);
	m_preset->resize (skin->getSize (Skin::BUTTON_EQ_PRESET));
	m_preset->move (skin->getPos (Skin::BUTTON_EQ_PRESET));
	m_preset->setEnabled(false); // FIXME: needs to be implemented

	connect(m_preset, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_preamp = new EqualizerSlider(this, -1);
	m_preamp->resize (skin->getSize (Skin::SLIDER_EQUALIZER_BGS));
	m_preamp->move(21, 38);
	connect (m_preamp, SIGNAL (sliderMoved (int)),
	         this, SLOT (updateServerPreamp (int)));

	for (int i=0; i < 10; i++) {
		m_bands[i] = new EqualizerSlider(this, i);
		m_bands[i]->resize (skin->getSize (Skin::SLIDER_EQUALIZER_BGS));
		m_bands[i]->move(78+i*18, 38);
		connect (m_bands[i], SIGNAL (numberedSliderMoved (int, int)),
		         this, SLOT (updateServerBands (int, int)));
	}

	connect (m_xconfig, SIGNAL (configChanged (QString, QString)),
	         this, SLOT (serverConfigValueChanged (QString, QString)));

	// we request the config values manually if we already are connected
	connect (m_xconfig, SIGNAL (configLoaded ()),
	         this, SLOT (loadServerConfig ()));
	if (m_xconfig->isReady()) {
		loadServerConfig ();
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

	/* Updade Buttons */
	m_closebtn->setIcon (skin->getIcon (Skin::BUTTON_EQ_CLOSE));
	m_shadebtn->setIcon (skin->getIcon (Skin::BUTTON_EQ_SHADE));
	m_enable->setIcon (skin->getIcon (Skin::BUTTON_EQ_ACTIVE));
	m_auto->setIcon (skin->getIcon (Skin::BUTTON_EQ_AUTO));
	m_preset->setIcon (skin->getIcon (Skin::BUTTON_EQ_PRESET));

	/* Update Sliders */
	QPixmap normal = skin->getItem (Skin::EQ_WIN_BAR_BTN_0);
	QPixmap pressed = skin->getItem (Skin::EQ_WIN_BAR_BTN_1);
	QPixmapList bgslist = skin->getBackgrounds (Skin::SLIDER_EQUALIZER_BGS);

	m_preamp->setSliders (normal, pressed);
	m_preamp->setBackground (bgslist);

	for (int i = 0; i < 10; i++) {
		m_bands[i]->setSliders (normal, pressed);
		m_bands[i]->setBackground (bgslist);
	}

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
//	r.setRect(86, 17, 113, 19);
	if (m_graph.height () > 0 ) {
		r.setRect(86, 17, m_graph.width (), m_graph.height ());
		paint.drawPixmap(r, m_graph, m_graph.rect());
	}

	paint.end();
}


bool
EqualizerWidget::haveEqualizerEffect()
{
	// FIXME: Disable Widget if doesn't get enabled
	// TODO: Test if server has equalizer effect plugin
	// TODO: Add 'don't bother me again' checkbox
	if (!(m_xconfig->values_get (QRegExp ("effect\\.order\\.\\d+")).
	                 contains ("equalizer"))) {
		int button = QMessageBox::information (this, "Equalizer not enabled",
		                      "You need to enable the equalizer plugin on\n" \
		                      "the server or the equalizer will not work\n\n" \
		                      "Should promoe enable the equalizer plugin?",
		                      "Enable", "Ignore");
		if (button == 0) {
			// Add 'equalizer' to first empty 'effect.order.<num>' entry,
			// xmms2d takes care that there alway is at least one empty entry
			int i = 0;
			QString key = "effect.order.%1";
			while (m_xconfig->has_key (key.arg (i))) {
				if (m_xconfig->value_get (key.arg (i)).isEmpty ()) {
					m_xconfig->value_set (key.arg (i), "equalizer");
					break;
				}
				i++;
			}
			return true;
		} else {
			return false;
		}
	}
	return true;

}

/*
 *  These methods handle server configuration updates and
 *  update the serverconfiguraten if we change something
 */
void
EqualizerWidget::loadServerConfig ()
{
	QString key;
	QString value;
	// set enabled checkbox
	key = QString ("equalizer.enabled");
	value = m_xconfig->value_get (key);
	serverConfigValueChanged (key, value);
	// set preamp
	key = QString ("equalizer.preamp");
	value = m_xconfig->value_get (key);
	serverConfigValueChanged (key, value);
	// Set band-sliders
	for (int i=0; i < 10; i++) {
		key = QString ("equalizer.legacy%1").arg(i);
		value = m_xconfig->value_get (key);
		serverConfigValueChanged (key, value);
	}
}

void
EqualizerWidget::serverConfigValueChanged (QString key, QString value)
{
	// FIXME: also test on use_legacy
	if (key.startsWith ("equalizer.enabled")) {
		if (value != "0") {
			m_enable->setChecked (true);
		} else {
			m_enable->setChecked (false);
		}
	}
	if (key == "equalizer.preamp") {
		// FIXME: value can be of type float
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
	if (enabled && haveEqualizerEffect()) {
		m_xconfig->value_set ("equalizer.enabled", "1");
		m_xconfig->value_set ("equalizer.use_legacy", "1");
	} else {
		m_enable->setChecked (false); // uncheck the button again if equalizer
		                              // effect is not enabled
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

#include "equalizerwidget.moc"

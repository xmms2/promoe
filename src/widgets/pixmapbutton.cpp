/**
 *  This file is a part of Promoe, an XMMS2 Client
 *
 *  Copyright (C) 2008 Thomas Frauendorfer
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#include "pixmapbutton.h"

#include <QIcon>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

void
PixmapButton::paintEvent( QPaintEvent * event )
{
	QPixmap pixmap = icon().pixmap (size(),
	                        isDown() ? QIcon::Active : QIcon::Normal,
							isChecked() ? QIcon::On : QIcon::Off);

	QPainter p;
	p.begin(this);
	p.drawPixmap( rect(), pixmap, pixmap.rect() );
	p.end();

}

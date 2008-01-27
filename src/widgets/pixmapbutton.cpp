

#include "pixmapbutton.h"

#include <QIcon>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

PixmapButton::PixmapButton (QWidget *parent) : QAbstractButton (parent)
{
}

PixmapButton::~PixmapButton ()
{
}

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

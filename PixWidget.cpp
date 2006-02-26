#include "MainWindow.h"
#include "PixWidget.h"

PixWidget::PixWidget (QWidget *parent) : QWidget (parent)
{
	Skin *s;
	MainWindow *mw = dynamic_cast<MainWindow *>(window ());
	if (!mw) {
		PlaylistWindow *pl = dynamic_cast<PlaylistWindow*>(window ());
		if (!pl) {
			qDebug ("What are you?!");
		}
		s = pl->getSkin ();
	} else {
		s = mw->getSkin ();
	}
	m_pixmap = QPixmap(0,0);
	connect (s, SIGNAL (skinChanged (Skin *)), this, SLOT (setPixmaps(Skin *)));
}


PixWidget::~PixWidget ()
{
}


void
PixWidget::setPixmaps(Skin *skin)
{
}

/*
 * Since almost every Widget I have here
 * is done by drawing self.pixmap onto self
 * we define this a generic PaintEvent handler
 */
void 
PixWidget::paintEvent (QPaintEvent *event)
{
	if (m_pixmap.isNull ()) {
		return;
	}

	QPainter (paint);
	paint.begin (this);
	paint.drawPixmap (rect (), m_pixmap, m_pixmap.rect ());
	paint.end ();
}

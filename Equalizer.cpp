#include "MainWindow.h"
#include "TitleBar.h"
#include "Equalizer.h"
#include "Button.h"

EqualizerWindow::EqualizerWindow (QWidget *parent) : QMainWindow (parent)
{
	m_mw = dynamic_cast<MainWindow *>(parent);

	setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);
	setAttribute (Qt::WA_DeleteOnClose);

	m_equalizer = new EqualizerWidget (this);
	m_equalizer->show();

	setCentralWidget (m_equalizer);

	setMinimumSize (275, 116);
	setMaximumSize (275, 116);
}

EqualizerWindow::~EqualizerWindow (void)
{
}

void
EqualizerWindow::setEnabled (void)
{
	qDebug ("test");
}


EqualizerWidget::EqualizerWidget (QWidget *parent) : QWidget (parent)
{
	Skin *skin = Skin::getInstance ();

	connect (skin, SIGNAL(skinChanged(Skin *)),
	         this, SLOT(setPixmaps(Skin *)));

	m_enable = new ToggleButton(this, Skin::EQ_WIN_ON_0, Skin::EQ_WIN_ON_1,
	                            Skin::EQ_WIN_OFF_0, Skin::EQ_WIN_OFF_1);
	m_enable->move(14, 18);

	connect(m_enable, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_auto = new ToggleButton(this, Skin::EQ_WIN_AUTO_ON_0, Skin::EQ_WIN_AUTO_ON_1,
	                          Skin::EQ_WIN_AUTO_OFF_0, Skin::EQ_WIN_AUTO_OFF_1);
	m_auto->move(39, 18);

	connect(m_auto, SIGNAL(clicked()), parent, SLOT(setEnabled()));

	m_preset = new Button(this, Skin::EQ_WIN_PRESET_0, Skin::EQ_WIN_PRESET_1);
	m_preset->move(217, 18);

	connect(m_preset, SIGNAL(clicked()), parent, SLOT(setEnabled()));
}

EqualizerWidget::~EqualizerWidget (void)
{
}

void
EqualizerWidget::mouseMoveEvent (QMouseEvent *event)
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

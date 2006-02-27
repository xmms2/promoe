#include "MainWindow.h"
#include "Skin.h"
#include "SkinChooser.h"

#include <QDir>
#include <QIcon>
#include <QFile>
#include <QSettings>

SkinChooser::SkinChooser (QWidget *parent) : QMainWindow (parent)
{

#ifndef _WIN32
	setWindowIcon (QIcon (":icon.png"));
#endif
	
	m_mw = dynamic_cast<MainWindow *>(parent);
	QWidget *c = new QWidget (this);
	setCentralWidget (c);

	m_vbox = new QVBoxLayout (c);
	QLabel *label = new QLabel ("Available skins...", c);
	label->setFont (QFont ("Helvetica", 16));
	m_vbox->addWidget (label);

	m_skin = new SkinList (c);
	m_vbox->addWidget (m_skin);

	resize (500, 300);

}

SkinList::SkinList (QWidget *parent) : QListWidget (parent)
{

	setIconSize (QSize (137, 58));

	QString path;
	path.append (QDir::homePath());
	path.append ("/.xmms2/clients/promoe/skins/");
	QDir d;

	d.setPath (path);
	d.setFilter (QDir::Dirs);

	QFileInfoList list = d.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QDir dir (fileInfo.filePath());
		QPixmap p = Skin::getPixmap ("main.bmp", dir);
		if (!p.isNull()) {
			new QListWidgetItem (QIcon (p), dir.dirName(), this);
		}
	}

	connect (this, SIGNAL (itemClicked (QListWidgetItem *)), this, SLOT (changeSkin (QListWidgetItem *)));
}

void
SkinList::changeSkin (QListWidgetItem *item)
{
	Skin *skin = Skin::getInstance ();

	QSettings settings;

	SkinChooser *sc = dynamic_cast<SkinChooser *>(window());
	qDebug ("change skin to %s", qPrintable (item->text()));

	skin->setSkin (QDir::homePath()+"/.xmms2/clients/promoe/skins/"+item->text());

	settings.setValue ("skin/path", QDir::homePath()+"/.xmms2/clients/promoe/skins/"+item->text());
}


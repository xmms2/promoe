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
	setWindowFlags (Qt::Dialog);
	setWindowModality (Qt::ApplicationModal);
	setAttribute (Qt::WA_DeleteOnClose);

	m_mw = dynamic_cast<MainWindow *>(parent);
	m_c = new QWidget (this);
	setCentralWidget (m_c);

	m_vbox = new QVBoxLayout (m_c);
	m_label = new QLabel ("Available skins...", m_c);
	m_label->setFont (QFont ("Helvetica", 16));
	m_vbox->addWidget (m_label);

	m_skin = new SkinList (m_c);
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
			
	new SkinChooserItem (QIcon (":CleanAMP/main.png"), "CleanAMP (default)", ":CleanAMP/", this);

	d.setPath (path);
	d.setFilter (QDir::Dirs);

	QFileInfoList list = d.entryInfoList();
	for (int i = 0; i < list.size(); ++i) {
		QFileInfo fileInfo = list.at(i);
		QDir dir (fileInfo.filePath());
		QPixmap p = Skin::getPixmap ("main", dir);
		if (!p.isNull()) {
			new SkinChooserItem (QIcon (p), dir.dirName(), dir.absolutePath(), this);
		}
	}

	connect (this, SIGNAL (itemClicked (QListWidgetItem *)), this, SLOT (changeSkin (QListWidgetItem *)));
}

void
SkinList::changeSkin (QListWidgetItem *item)
{
	Skin *skin = Skin::getInstance ();
	SkinChooserItem *it = dynamic_cast<SkinChooserItem*> (item);

	QSettings settings;

	skin->setSkin (it->getPath ());
	settings.setValue ("skin/path", it->getPath ());
}


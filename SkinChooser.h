#include "MainWindow.h"

#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>

class SkinList : public QListWidget
{
	Q_OBJECT
	public:
		SkinList (QWidget *parent);
		~SkinList () {}

	public slots:
		void changeSkin (QListWidgetItem *item);
};

class SkinChooser : public QMainWindow
{
	public:
		SkinChooser (QWidget *parent);
		~SkinChooser (void);
		MainWindow *getMW (void) { return m_mw; }

	private:
		QVBoxLayout *m_vbox;
		SkinList *m_skin;
		QLabel *m_label;
		QWidget *m_c;
		MainWindow *m_mw;

};


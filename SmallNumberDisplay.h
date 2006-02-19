#include "PixWidget.h"
#include <QPainter>


class SmallNumberDisplay : public PixWidget
{
	Q_OBJECT;

	public:
		SmallNumberDisplay (QWidget *parent, int w);
		~SmallNumberDisplay () { };

		void setNumber (int num, int len);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		char m_nums[2];
		int m_num;
		int m_w;

		void drawNumber (void);

		Skin *m_skin;
};


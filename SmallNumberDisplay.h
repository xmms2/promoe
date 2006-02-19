#include "PixWidget.h"
#include <QPainter>


class SmallNumberDisplay : public PixWidget
{
	Q_OBJECT;

	public:
		SmallNumberDisplay (QWidget *parent, int w);
		~SmallNumberDisplay () { };

		void setNumber (char i1, char i2, char i3);
		void setNumber (char i1, char i2);

	public slots:
		void setPixmaps (Skin *skin);

	private:
		char m_nums[2];
		int m_num;
		int m_w;

		void drawNumber (void);

		Skin *m_skin;
};


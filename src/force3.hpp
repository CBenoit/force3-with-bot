#ifndef FORCE3_HPP
#define FORCE3_HPP

#include <QMainWindow>

namespace Ui {
class Force3;
}

class Force3 : public QMainWindow
{
	Q_OBJECT

public:
	explicit Force3(QWidget* parent = 0);
	~Force3();

private:
	Ui::Force3* ui;
};

#endif // FORCE3_HPP

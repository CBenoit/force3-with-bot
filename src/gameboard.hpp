#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <QWidget>

namespace Ui {
class Gameboard;
}

class Gameboard : public QWidget
{
	Q_OBJECT

public:
	explicit Gameboard(QWidget* parent = 0);
	~Gameboard();

private:
	Ui::Gameboard* m_ui;
};

#endif // GAMEBOARD_HPP

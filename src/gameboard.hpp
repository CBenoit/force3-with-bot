#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <QWidget>


namespace Ui {
class Gameboard;
}
class QGridLayout;

class Gamesquare;

namespace square {
enum class type;
}

class Gameboard : public QWidget
{
	Q_OBJECT

public:
	explicit Gameboard(QWidget* parent = nullptr);
	Gameboard(const Gameboard&) = delete;
	Gameboard& operator=(const Gameboard&) = delete;
	~Gameboard();

	void square(unsigned char x, unsigned char y, square::type pawn_type);
	square::type square(unsigned char x, unsigned char y) const;

	void swap(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

protected:
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	void draw();

	std::array<std::array<Gamesquare*,3>,3> m_squares;
	QGridLayout* m_layout;
	Ui::Gameboard* m_ui;
};

#endif // GAMEBOARD_HPP

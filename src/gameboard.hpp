#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Breep project which is released under the                                //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <QWidget>

class QGridLayout;

class Gamesquare;

namespace square {
enum class type: unsigned char;
}

class QPoint;

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
	
private slots:
	void gamesquare_pressed(int x, int y);
	void gamesquare_released(int x, int y);

protected:
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	void draw();
	void next_turn();

	std::array<std::array<Gamesquare*,3>,3> m_squares;
	QGridLayout* m_layout;
	QPoint m_last_square_pressed;
	square::type m_current_player;
};

#endif // GAMEBOARD_HPP

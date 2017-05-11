///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <QResizeEvent>
#include <QGridLayout>
#include <cmath>

#include "gameboard.hpp"
#include "boardsquare.hpp"
#include "gamesquare.hpp"
#include "move.hpp"

Gameboard::Gameboard(QWidget *parent) :
	QWidget(parent),
	m_layout(new QGridLayout),
	m_last_square_pressed(-1,-1),
	m_game_state()
{

	for (unsigned char i{3} ; i-- ;) {
		for (unsigned char j{3} ; j-- ;) {
			m_squares[i][j] = new Gamesquare(QPoint(i,j));
			connect(m_squares[i][j], &Gamesquare::pressed, this, &Gameboard::gamesquare_pressed);
			connect(m_squares[i][j], &Gamesquare::released, this, &Gameboard::gamesquare_released);
			m_layout->addWidget(m_squares[i][j], j, i);
		}
	}
	m_squares[1][1]->type(square::type::empty_square);
	this->setLayout(m_layout);
	draw();
	show();
}

Gameboard::~Gameboard() {
	for (unsigned char i{3} ; i-- ;) {
		for (unsigned char j{3} ; j-- ;) {
			delete m_squares[i][j];
		}
	}
	delete m_layout;
}

void Gameboard::square(unsigned char x, unsigned char y, square::type pawn_type) {
	m_squares[x][y]->type(pawn_type);
}

square::type Gameboard::square(unsigned char x, unsigned char y) const {
	return m_squares[x][y]->type();
}

void Gameboard::swap(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
	m_squares[x1][y1]->swap(*m_squares[x2][y2]);
}

void Gameboard::gamesquare_pressed(int x, int y) {
	m_last_square_pressed = QPoint(x, y);
}

void Gameboard::gamesquare_released(int x, int y) {
	if (x >= 0 && x <= 2 && y >= 0 && y <= 2) {
		Gamesquare* square = m_squares[m_last_square_pressed.x()][m_last_square_pressed.y()];
		Gamesquare* target = m_squares[x][y];
		if (square == target) { // put a token
			if (m_game_state.play(move::SetColor(x, y))) {
				square->type(m_game_state.get_previous_player());
			}
		} else if (target->type() == square::type::available
					   && square->type() != square::type::available
					   && square->type() != square::type::empty_square) { // move a token on any free square
			if (m_game_state.play(move::Swap(m_last_square_pressed.x(), m_last_square_pressed.y(), x, y))) {
				square->swap(*target);
			}
		} else if (target->type() == square::type::empty_square) { // slides
			int x_diff = std::abs(m_last_square_pressed.x() - x);
			int y_diff = std::abs(m_last_square_pressed.y() - y);
			if (m_game_state.play(move::Slide(m_last_square_pressed.x(), m_last_square_pressed.y(), x, y))) {
				if (x_diff + y_diff == 1) { // slide one square
					square->swap(*target);
				} if (x_diff == 2 && y_diff == 0) { // slide two squares horizontally
					target->swap(*m_squares[1][y]);
					square->swap(*m_squares[1][y]);
				} else if (y_diff == 2 && x_diff == 0) { // slide two squares vertically
					target->swap(*m_squares[x][1]);
					square->swap(*m_squares[x][1]);
				}
			}
		}
	}
}

void Gameboard::resizeEvent(QResizeEvent*) {
	draw();
}

void Gameboard::draw() {
	const QSize childen_size(size().width() / 3, size().height() / 3);
	for (unsigned char i{3} ; i-- ;) {
		for (unsigned char j{3} ; j-- ;) {
			m_squares[i][j]->resize(childen_size);
		}
	}
}


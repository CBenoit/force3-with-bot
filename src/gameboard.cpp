#include "gameboard.hpp"
#include "ui_gameboard.h"
#include "gamesquare.hpp"

#include <QResizeEvent>
#include <QPoint>
#include <QGridLayout>
#include <cmath>

Gameboard::Gameboard(QWidget *parent) :
	QWidget(parent),
	m_layout(new QGridLayout),
	m_last_square_pressed(-1,-1),
	m_current_player(square::type::blue)
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
	if (x >= 0 && x <= 3 && y >= 0 && y <= 3) {
		Gamesquare* square = m_squares[m_last_square_pressed.x()][m_last_square_pressed.y()];
		Gamesquare* target = m_squares[x][y];
		if (square == target) {
			if (square->type() == square::type::available) {
				square->type(m_current_player);
				next_turn();
			}
		} else {
			int x_diff = std::abs(m_last_square_pressed.x() - x);
			int y_diff = std::abs(m_last_square_pressed.y() - y);
			if (x_diff + y_diff == 1) {
				if (target->type() == square::type::empty_square) {
					square->swap(*target);
					next_turn();
				} else if (target->type() == square::type::available
						   && square->type() != square::type::available
						   && square->type() != square::type::empty_square){
					square->swap(*target);
					next_turn();
				}
			} else if (x_diff == 2 && y_diff == 0) {
				if (target->type() == square::type::empty_square) {
					target->swap(*m_squares[1][y]);
					square->swap(*m_squares[1][y]);
				}
			} else if (y_diff == 2 && x_diff == 0) {
				if (target->type() == square::type::empty_square) {
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

void Gameboard::next_turn() {
	if (m_current_player == square::type::red) {
		m_current_player = square::type::blue;
	} else {
		m_current_player = square::type::red;
	}
}

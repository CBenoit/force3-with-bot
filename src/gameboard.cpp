///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include <QResizeEvent>
#include <QGridLayout>
#include <QTimer>
#include <iostream>

#include "gameboard.hpp"


std::pair<uint, heuristic::function_t> Gameboard::blue_brain = {0, &heuristic::easy};
std::pair<uint, heuristic::function_t> Gameboard::red_brain = {0, &heuristic::easy};

unsigned char Gameboard::blue_depth = 3;
unsigned char Gameboard::red_depth = 3;

bool Gameboard::blue_is_ai = false;
bool Gameboard::red_is_ai = true;

Gameboard::Gameboard(QWidget *parent) :
	QWidget(parent),
	m_blue_turn(true),
	m_game_finished(false),
	m_layout(new QGridLayout),
	m_last_square_pressed(-1,-1),
	m_game_state(),
	m_blue_ai(blue_brain.second, blue_depth),
	m_red_ai(red_brain.second, red_depth)
{
	setAutoFillBackground(true);
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

	m_blue_turn = !m_blue_turn;
	next_turn();
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
	setPalette(QPalette(square::COLORS[static_cast<unsigned char>(m_blue_turn ? square::type::blue : square::type::red)]));
	if (ia_turn()) {
		return;
	}

	if (x >= 0 && x < BOARD_DIMENSION && y >= 0 && y < BOARD_DIMENSION) {
		square::type from{m_game_state.get_board_state().get(m_last_square_pressed.x(), m_last_square_pressed.y())};
		square::type target{m_game_state.get_board_state().get(x, y)};
		uint_fast8_t to_x = static_cast<uint_fast8_t>(x);
		uint_fast8_t to_y = static_cast<uint_fast8_t>(y);
		if (x == m_last_square_pressed.x() && y == m_last_square_pressed.y()) { // put a token
			move::SetColor set_color{to_x, to_y};
			if (m_game_state.play(set_color)) {
				play(std::move(set_color));
				next_turn();
			}
		} else if (target == square::type::available
				   && from != square::type::available
				   && from != square::type::empty_square) { // move a token on any free square
			move::Swap swap{static_cast<uint_fast8_t>(m_last_square_pressed.x()), static_cast<uint_fast8_t>(m_last_square_pressed.y()),
						to_x, to_y};
			if (m_game_state.play(swap)) {
				play(std::move(swap));
				next_turn();
			}
		} else if (target == square::type::empty_square) { // slides
			move::Slide slide{static_cast<uint_fast8_t>(m_last_square_pressed.x()), static_cast<uint_fast8_t>(m_last_square_pressed.y()),
						to_x, to_y};
			if (m_game_state.play(slide)) {
				play(std::move(slide));
				next_turn();
			}
		}
	}
}

void Gameboard::AI_play() {
	move::MoveWrapper move;
	if (m_blue_turn) {
		move = m_blue_ai.think(m_game_state);
	} else {
		move = m_red_ai.think(m_game_state);
	}

	if (move.is_set_color()) {
		if (m_game_state.play(move.unwrap_set_color())) {
			play(move.unwrap_set_color());
		}
	} else if (move.is_slide()) {
		if (m_game_state.play(move.unwrap_slide())) {
			play(move.unwrap_slide());
		}
	} else if (move.is_swap()) {
		if (m_game_state.play(move.unwrap_swap())) {
			play(move.unwrap_swap());
		}
	}

	next_turn();
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

void Gameboard::play(move::Slide slide) {
	Gamesquare* square = m_squares[slide.from_x][slide.from_y];
	Gamesquare* target = m_squares[slide.to_x][slide.to_y];

	int x_diff = std::abs(slide.from_x - slide.to_x);
	int y_diff = std::abs(slide.from_y - slide.to_y);
	if (x_diff + y_diff == 1) { // slide one square
		square->swap(*target);
	} if (x_diff == 2 && y_diff == 0) { // slide two squares horizontally
		target->swap(*m_squares[1][slide.to_y]);
		square->swap(*m_squares[1][slide.to_y]);
	} else if (y_diff == 2 && x_diff == 0) { // slide two squares vertically
		target->swap(*m_squares[slide.to_x][1]);
		square->swap(*m_squares[slide.to_x][1]);
	}
}

void Gameboard::play(move::Swap swap) {
	Gamesquare* square = m_squares[swap.from_x][swap.from_y];
	Gamesquare* target = m_squares[swap.to_x][swap.to_y];
	square->swap(*target);
}

void Gameboard::play(move::SetColor set_color) {
	Gamesquare* square = m_squares[set_color.x][set_color.y];
	square->type(m_game_state.get_previous_player());
}

void Gameboard::next_turn() {
	if (m_game_finished) {
		return;
	}

	if (m_game_state.is_there_a_winner()) {
		for (unsigned char i{3} ; i-- ;) {
			for (unsigned char j{3} ; j-- ;) {
				disconnect(m_squares[i][j], &Gamesquare::pressed, this, &Gameboard::gamesquare_pressed);
				disconnect(m_squares[i][j], &Gamesquare::released, this, &Gameboard::gamesquare_released);
			}
		}

		m_game_finished = true;
		emit winner_detected(m_blue_turn);
		return;
	}

	m_blue_turn = !m_blue_turn;
	setPalette(QPalette(square::COLORS[static_cast<unsigned char>(m_blue_turn ? square::type::blue : square::type::red)]));

	if (ia_turn()) {
		QTimer::singleShot(100, this, SLOT(AI_play()));
	}
}

bool Gameboard::ia_turn() const {
	return m_blue_turn ? blue_is_ai : red_is_ai;
}

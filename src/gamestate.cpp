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

#include "gamestate.hpp"
#include "gamesquare.hpp"

using square::type;

const square::type GameState::PLAYER_TURNS[static_cast<int>(square::type::size) + 1] =
							{ COLORS_AND_PLAYERS(AS_EXT_TYPE) square::type::red };

GameState::GameState()
	: m_remaining_tokens{BOARD_DIMENSION, BOARD_DIMENSION},
	  m_current_player{type::blue},
	  m_board_state{{type::available, type::available,    type::available},
					{type::available, type::empty_square, type::available},
					{type::available, type::available,    type::available}}
{
	static_assert(static_cast<int>(type::red) + 1 == static_cast<int>(type::blue),"blue must follow red in square::type");
	static_assert(static_cast<int>(type::blue) + 1 == static_cast<int>(type::size), "blue must be the last element of square:type");
}

// ============ generate_neighbours part ================ //

template <typename Move>
GameState&& generate_game_state_from_move(GameState current_game_state, Move move) {
	current_game_state.do_play(move);
	current_game_state.next_turn();
	return std::move(current_game_state);
}

std::vector<GameState> GameState::generate_neighbours() const {
	std::vector<GameState> neighbours;
	neighbours.reserve(19); // there are at most 19 differents neighbours given the current state.

	for (uint_fast8_t i{BOARD_DIMENSION * BOARD_DIMENSION}; i--;) {
		type square_type = m_board_state.get(i);
		if (square_type == type::empty_square) { // slides
			uint_fast8_t empty_square_x = i % BOARD_DIMENSION, empty_square_y = i / BOARD_DIMENSION;
			for (uint_fast8_t x{BOARD_DIMENSION}; x--;) {
				for (uint_fast8_t y{BOARD_DIMENSION}; y--;) {
					move::Slide slide{x, y, empty_square_x, empty_square_y};
					if (is_valid_move(slide)) {
						neighbours.push_back(generate_game_state_from_move(*this, slide));
					}
				}
			}
		} else if (square_type == m_current_player) { // swaps
			uint_fast8_t from_x = i % BOARD_DIMENSION, from_y = i / BOARD_DIMENSION;
			for (uint_fast8_t x{BOARD_DIMENSION}; x--;) {
				for (uint_fast8_t y{BOARD_DIMENSION}; y--;) {
					move::Swap swap{from_x, from_y, x, y};
					if (is_valid_move(swap)) {
						neighbours.push_back(generate_game_state_from_move(*this, swap));
					}
				}
			}
		} else if (square_type == type::available && has_remaining_tokens(m_current_player)) { // set color
			uint_fast8_t x = i % BOARD_DIMENSION, y = i / BOARD_DIMENSION;
			move::SetColor set_color{x, y};
			if (is_valid_move(set_color)) {
				neighbours.push_back(generate_game_state_from_move(*this, set_color));
			}
		}
	}

	return std::move(neighbours);
}
// ========= end generate_neighbours part =============== //

bool GameState::is_there_a_winner(square::type* winner) const {
	square::type& st = *winner;
	for (uint_fast8_t i{0} ; i < 7 ; i+=3) {
		st = m_board_state.get(i);
		if ((st == square::type::red || st == square::type::blue)
			&& m_board_state.get(i+1) == st && m_board_state.get(i+2) == st) {
			return true;
		}
	}

	for (uint_fast8_t i{3} ; i-- ;) {
		st = m_board_state.get(i);
		if ((st == square::type::red || st == square::type::blue)
			&& m_board_state.get(i+3) == st && m_board_state.get(i+6) == st) {
			return true;
		}
	}

	st = m_board_state.get(4);
	if (st == square::type::red || st == square::type::blue) {
		if (m_board_state.get(0) == st && m_board_state.get(8) == st) {
			return true;
		}

		if (m_board_state.get(2) == st && m_board_state.get(6) == st) {
			return true;
		}
	}
	return false;
}

void GameState::do_play(move::Slide slide) {
	uint_fast8_t x_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(slide.from_x - slide.to_x)));
	uint_fast8_t y_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(slide.from_y - slide.to_y)));
	if (x_diff + y_diff == 1) { // slide one square
		swap(slide.from_x, slide.from_y, slide.to_x, slide.to_y);
	} else if (x_diff == 2 && y_diff == 0) { // slide two squares on x
		swap(slide.to_x, slide.to_y, 1, slide.to_y);
		swap(slide.from_x, slide.from_y, 1, slide.to_y);
	} else if (y_diff == 2 && x_diff == 0) { // slide two squares on y
		swap(slide.to_x, slide.to_y, slide.to_x, 1);
		swap(slide.from_x, slide.from_y, slide.to_x, 1);
	}

	m_last_move.set_move(slide);
}

bool GameState::is_valid_move(move::Slide slide) const {
	if (m_board_state.get(slide.to_x, slide.to_y) == square::type::empty_square) {
		if (m_last_move.is_slide()) { // doing reverse slide is not authorized.
			move::Slide previous_slide = m_last_move.unwrap_slide();
			if (previous_slide.from_x == slide.to_x && previous_slide.from_y == slide.to_y
				&& previous_slide.to_x == slide.from_x && previous_slide.to_y == slide.from_y) {
				return false;
			}
		}

		uint_fast8_t x_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(slide.from_x - slide.to_x)));
		uint_fast8_t y_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(slide.from_y - slide.to_y)));
			   //slide one square      slide two squares on x or y
		return (x_diff + y_diff == 1) || (x_diff != y_diff && x_diff + y_diff == 2);
	}
	return false;
}

#include <cmath>

#include "gamestate.hpp"
#include "gamesquare.hpp"
#include "move.hpp"

#define BOARD_DIMENSION 3

using square::type;

GameState::GameState()
	: m_remaining_blue_tokens(BOARD_DIMENSION),
	  m_remaining_red_tokens(BOARD_DIMENSION),
	  m_current_player(type::blue),
	  m_gameboard_state({{type::available, type::available,    type::available},
						 {type::available, type::empty_square, type::available},
						 {type::available, type::available,    type::available}})
{}

GameState::GameState(const GameState& game_manager)
	: m_remaining_blue_tokens(game_manager.m_remaining_blue_tokens),
	  m_remaining_red_tokens(game_manager.m_remaining_red_tokens),
	  m_current_player(game_manager.m_current_player),
	  m_gameboard_state(game_manager.m_gameboard_state)
{}

GameState& GameState::operator=(const GameState& game_manager) {
	m_remaining_blue_tokens = game_manager.m_remaining_blue_tokens;
	m_remaining_red_tokens = game_manager.m_remaining_red_tokens;
	m_current_player = game_manager.m_current_player;
	m_gameboard_state = game_manager.m_gameboard_state;

	return *this;
}

GameState::~GameState() {}

// ============ generate_neighbours part ================ //

template <typename Move>
GameState generate_game_state_from_move(const GameState& current_game_state, const Move& move) {
	GameState new_game_state(current_game_state);
	new_game_state.impl_play(move);
	new_game_state.impl_next_turn();
	return std::move(new_game_state);
}

std::vector<GameState> GameState::generate_neighbours() const {
	std::vector<GameState> neighbours;
	neighbours.reserve(19); // there are at most 19 differents neighbours given the current state.

	for (unsigned int i = BOARD_DIMENSION * BOARD_DIMENSION; i--;) {
		type square_type = m_gameboard_state.get(i);
		if (square_type == type::empty_square) { // slides
			int empty_square_x = i % BOARD_DIMENSION, empty_square_y = i / BOARD_DIMENSION;
			for (int x = BOARD_DIMENSION; x--;) {
				for (int y = BOARD_DIMENSION; y--;) {
					move::Slide slide{x, y, empty_square_x, empty_square_y};
					if (impl_is_valid_move(slide)) {
						neighbours.push_back(generate_game_state_from_move(*this, slide));
					}
				}
			}
		} else if (square_type == m_current_player) { // swaps
			int from_x = i % BOARD_DIMENSION, from_y = i / BOARD_DIMENSION;
			for (int x = BOARD_DIMENSION; x--;) {
				for (int y = BOARD_DIMENSION; y--;) {
					move::Swap swap{from_x, from_y, x, y};
					if (impl_is_valid_move(swap)) {
						neighbours.push_back(generate_game_state_from_move(*this, swap));
					}
				}
			}
		} else if (square_type == type::available && has_remaining_tokens(m_current_player)) { // set color
			int x = i % BOARD_DIMENSION, y = i / BOARD_DIMENSION;
			move::SetColor set_color{x, y};
			if (impl_is_valid_move(set_color)) {
				neighbours.push_back(generate_game_state_from_move(*this, set_color));
			}
		}
	}

	return std::move(neighbours);
}

// ========= end generate_neighbours part =============== //

void GameState::impl_play(const move::Slide& slide) {
	int x_diff = std::abs(slide.from_x - slide.to_x);
	int y_diff = std::abs(slide.from_y - slide.to_y);
	if (x_diff + y_diff == 1) { // slide one square
		swap(slide.from_x, slide.from_y, slide.to_x, slide.to_y);
	} else if (x_diff == 2 && y_diff == 0) { // slide two squares on x
		swap(slide.to_x, slide.to_y, 1, slide.to_y);
		swap(slide.from_x, slide.from_y, 1, slide.to_y);
	} else if (y_diff == 2 && x_diff == 0) { // slide two squares on y
		swap(slide.to_x, slide.to_y, slide.to_x, 1);
		swap(slide.from_x, slide.from_y, slide.to_x, 1);
	}
}

void GameState::impl_play(const move::Swap& swp) {
	swap(swp.x1, swp.y1, swp.x2, swp.y2);
}

void GameState::impl_play(const move::SetColor& set_color) {
	decrement_remaining_tokens(m_current_player);
	m_gameboard_state.set(set_color.x, set_color.y, m_current_player);
}

bool GameState::impl_is_valid_move(const move::Slide& slide) const {
	if (m_gameboard_state.get(slide.to_x, slide.to_y) == square::type::empty_square) {
		int x_diff = std::abs(slide.from_x - slide.to_x);
		int y_diff = std::abs(slide.from_y - slide.to_y);
		if (x_diff + y_diff == 1) { // slide one square
			return true;
		} else if (x_diff == 2 && y_diff == 0) { // slide two squares on x
			return true;
		} else if (y_diff == 2 && x_diff == 0) { // slide two squares on y
			return true;
		}
	}
	return false;
}

bool GameState::impl_is_valid_move(const move::Swap& swp) const {
	return m_gameboard_state.get(swp.x1, swp.y1) == m_current_player && m_gameboard_state.get(swp.x2, swp.y2) == type::available;
}

bool GameState::impl_is_valid_move(const move::SetColor& set_color) const {
	return m_gameboard_state.get(set_color.x, set_color.y) == type::available && has_remaining_tokens(m_current_player) != 0;
}

void GameState::impl_next_turn() {
	if (m_current_player == type::red) {
		m_current_player = type::blue;
	} else {
		m_current_player = type::red;
	}
}

// helpers

bool GameState::has_remaining_tokens(square::type square_type) const {
	return (square_type == type::blue && m_remaining_blue_tokens)
			|| (square_type == type::red && m_remaining_red_tokens);
}

void GameState::decrement_remaining_tokens(square::type square_type) {
	if (square_type == type::blue) {
		--m_remaining_blue_tokens;
	} else if (square_type == type::red) {
		--m_remaining_red_tokens;
	}
}

void GameState::swap(int x1, int y1, int x2, int y2) {
	type tmp = m_gameboard_state.get(x1, y1);
	m_gameboard_state.set(x1, y1, m_gameboard_state.get(x2, y2));
	m_gameboard_state.set(x2, y2, tmp);
}

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

#include <iostream>
void find_empty_square(const GameboardState& gameboard_state, int* x, int* y) {
	for (unsigned int i = BOARD_DIMENSION * BOARD_DIMENSION; i--;) {
		if (gameboard_state.get(i) == type::empty_square) {
			*x = i % BOARD_DIMENSION;
			*y = i / BOARD_DIMENSION;
			return;
		}
	}
}

std::vector<GameState> GameState::generate_neighbours() const {
	std::vector<GameState> neighbours;
	// once every tokens are on the board, there is always 4 slides possibles and 6 * 2 swaps possibles.
	neighbours.reserve(16);

	// FIXME: finish this function.

	int empty_square_x, empty_square_y;
	find_empty_square(m_gameboard_state, &empty_square_x, &empty_square_y);

	return std::move(neighbours);
}

// ========= end generate_neighbours part =============== //

bool GameState::impl_play(move::Slide slide) {
	if (m_gameboard_state.get(slide.to_x, slide.to_y) == square::type::empty_square) {
		int x_diff = std::abs(slide.from_x - slide.to_x);
		int y_diff = std::abs(slide.from_y - slide.to_y);
		if (x_diff + y_diff == 1) { // slide one square
			swap(slide.from_x, slide.from_y, slide.to_x, slide.to_y);
			return true;
		} else if (x_diff == 2 && y_diff == 0) { // slide two squares on x
			swap(slide.to_x, slide.to_y, 1, slide.to_y);
			swap(slide.from_x, slide.from_y, 1, slide.to_y);
			return true;
		} else if (y_diff == 2 && x_diff == 0) { // slide two squares on y
			swap(slide.to_x, slide.to_y, slide.to_x, 1);
			swap(slide.from_x, slide.from_y, slide.to_x, 1);
			return true;
		}
	}
	return false;
}

bool GameState::impl_play(move::Swap swp) {
	if ((m_gameboard_state.get(swp.x1, swp.y1) == type::blue || m_gameboard_state.get(swp.x1, swp.y1) == type::red)
		&& m_gameboard_state.get(swp.x2, swp.y2) == type::available) {
		// move is OK!
		swap(swp.x1, swp.y1, swp.x2, swp.y2);
		return true;
	}
	// otherwise, the move is not OK!
	return false;
}

bool GameState::impl_play(move::SetColor set_color) {
	if (m_gameboard_state.get(set_color.x, set_color.y) == type::available && has_remaining_tokens(m_current_player)) {
		// move is OK!
		decrement_remaining_tokens(m_current_player);
		m_gameboard_state.set(set_color.x, set_color.y, m_current_player);
		return true;
	}
	// otherwise, the move is not OK!
	return false;
}

void GameState::impl_next_turn() {
	if (m_current_player == type::red) {
		m_current_player = type::blue;
	} else {
		m_current_player = type::red;
	}
}

// helpers

bool GameState::has_remaining_tokens(square::type square_type) {
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

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FORCE3_GAMEMANAGER_HPP
#define FORCE3_GAMEMANAGER_HPP

#include <vector>
#include <cstdint>

#include "boardstate.hpp"
#include "gamesquare.hpp"
#include "boardsquare.hpp"
#include "move.hpp"

// forward declaration
namespace move {
struct Slide;
struct Swap;
struct SetColor;
}

class GameState
{
public:
	GameState();
	GameState(const GameState& game_manager) = default;
	GameState(GameState&&) = default;
	GameState& operator=(const GameState& game_manager) = default;
	~GameState() = default;

	template <typename Move>
	bool play(const Move& move) {
		if (is_valid_move(move)) {
			do_play(move);
			next_turn();
			return true;
		}
		return false;
	}

	square::type get_current_player() const {
		return m_current_player;
	}

	square::type get_previous_player() const {
		return PLAYER_TURNS[static_cast<int>(m_current_player) + 1];
	}

	const BoardState& get_board_state() const {
		return m_board_state;
	}

	std::vector<GameState> generate_neighbours() const;

private:
	void do_play(move::Slide slide);
	void do_play(move::Swap swp) {
		swap(swp.x1, swp.y1, swp.x2, swp.y2);
	}
	void do_play(move::SetColor set_color) {
		decrement_remaining_tokens(m_current_player);
		m_board_state.set(set_color.x, set_color.y, m_current_player);
	}

	bool is_valid_move(move::Slide slide) const;
	bool is_valid_move(move::Swap swp) const {
		return m_board_state.get(swp.x1, swp.y1) == m_current_player && m_board_state.get(swp.x2, swp.y2) == square::type::available;
	}
	bool is_valid_move(move::SetColor set_color) const {
		return m_board_state.get(set_color.x, set_color.y) == square::type::available && has_remaining_tokens(m_current_player) != 0;
	}

	void next_turn()  {
		m_current_player = PLAYER_TURNS[static_cast<int>(m_current_player) + 1];
	}

	// helpers

	bool has_remaining_tokens(square::type square_type) const {
		return m_remaining_tokens[static_cast<int>(square_type) - static_cast<int>(square::type::red)];
	}

	void decrement_remaining_tokens(square::type square_type)  {
		--m_remaining_tokens[static_cast<int>(square_type) - static_cast<int>(square::type::red)];
	}

	void swap(uint_fast8_t x1, uint_fast8_t y1, uint_fast8_t x2, uint_fast8_t y2) {
		square::type tmp = m_board_state.get(x1, y1);
		m_board_state.set(x1, y1, m_board_state.get(x2, y2));
		m_board_state.set(x2, y2, tmp);
	}

	// attributes

	uint_fast8_t m_remaining_tokens[2];

	square::type m_current_player;
	BoardState m_board_state;

	static const square::type PLAYER_TURNS[static_cast<int>(square::type::size) + 1];

	template <typename Move>
	friend GameState&& generate_game_state_from_move(GameState current_game_state, Move move);
};

#endif // FORCE3_GAMEMANAGER_HPP

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
#include <cstddef>

#include "boardstate.hpp"
#include "gamesquare.hpp"
#include "boardsquare.hpp"
#include "move.hpp"
#include "movewrapper.hpp"

class GameState
{
public:
	GameState();
	GameState(const GameState&) = default;
	GameState(GameState&&) = default;
	GameState& operator=(const GameState&) = default;
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

	square::type get_current_player() const { return m_current_player; }

	square::type get_previous_player() const { return PLAYER_TURNS[static_cast<int>(m_current_player) + 1]; }

	move::MoveWrapper get_last_move() const { return m_last_move; }

	BoardState get_board_state() const { return m_board_state; }

	std::vector<GameState> generate_neighbours() const;

	bool is_there_a_winner(square::type* winner) const;
	bool is_there_a_winner(nullptr_t) const {
		return is_there_a_winner();
	}
	bool is_there_a_winner() const {
		square::type t;
		return is_there_a_winner(&t);
	}

	static square::type opposite_player(square::type t) {
		return PLAYER_TURNS[static_cast<int>(t) + 1];
	}

private:
	void do_play(move::Slide slide);
	void do_play(move::Swap swp) {
		swap(swp.from_x, swp.from_y, swp.to_x, swp.to_y);
		m_last_move.set_move(swp);
	}
	void do_play(move::SetColor set_color) {
		decrement_remaining_tokens(m_current_player);
		m_board_state.set(set_color.x, set_color.y, m_current_player);
		m_last_move.set_move(set_color);
	}

	bool is_valid_move(move::Slide slide) const;
	bool is_valid_move(move::Swap swp) const {
		return m_board_state.get(swp.from_x, swp.from_y) == m_current_player && m_board_state.get(swp.to_x, swp.to_y) == square::type::available;
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
		uint_fast8_t idx1 = x1 + y1 * 3;
		uint_fast8_t idx2 = x2 + y2 * 3;
		square::type tmp = m_board_state.get(idx1);
		m_board_state.set(idx1, m_board_state.get(idx2));
		m_board_state.set(idx2, tmp);
	}

	// attributes

	uint_fast8_t m_remaining_tokens[2];

	square::type m_current_player;
	BoardState m_board_state;

	move::MoveWrapper m_last_move;

	static const square::type PLAYER_TURNS[static_cast<int>(square::type::size) + 1];

	template <typename Move>
	friend GameState&& generate_game_state_from_move(GameState current_game_state, Move move);
};

#endif // FORCE3_GAMEMANAGER_HPP

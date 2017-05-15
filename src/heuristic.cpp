#include <cstdint>

#include "heuristic.hpp"
#include "boardstate.hpp"

static inline heuristic::return_t align_value(square::type player, square::type st0, square::type st1, square::type st2) {
	if (st0 == player) {
		if (st1 == player) {
			if (st2 == player) {
				return 10000;
			} else if (st2 == square::type::available) {
				return 21;
			} else if (st2 == square::type::empty_square) {
				return 16;
			} else {
				return 11;
			}
		} else if (st2 == player) {
			if (st1 == square::type::available) {
				return 21;
			} else if (st1 == square::type::empty_square) {
				return 16;
			} else {
				return 11;
			}
		} else {
			return 5;
		}
	} else if (st1 == player) {
		if (st2 == player) {
			if (st0 == square::type::available) {
				return 21;
			} else if (st0 == square::type::empty_square) {
				return 16;
			} else {
				return 11;
			}
		} else {
			return 5;
		}
	} else if (st2 == player) {
		return 5;
	}
	return 0;
}

auto heuristic::improved_heuristic(const GameState& gs, square::type player) -> return_t {

	square::type opponent = GameState::opposite_player(player);
	return_t value{0};

	BoardState bs{gs.get_board_state()};
	for (uint_fast8_t i{BOARD_DIMENSION} ; i-- ;) {

		square::type vi_0 = bs.get(i), vi_1 = bs.get(i + 3), vi_2 = bs.get(i + 6);
		square::type v0_i = bs.get(i*3), v1_i = bs.get(1,i), v2_i = bs.get(2,i);

		// vert
		value += align_value(player, vi_0, vi_1, vi_2);
		value -= align_value(opponent, vi_0, vi_1, vi_2);

		// hzt
		value += align_value(player, v0_i,  v1_i, v2_i);
		value -= align_value(opponent, v0_i,  v1_i, v2_i);
	}
	// diagonals
	value += align_value(player, bs.get(0), bs.get(4), bs.get(8));
	value += align_value(player, bs.get(2), bs.get(4), bs.get(6));
	value -= align_value(opponent, bs.get(0), bs.get(4), bs.get(8));
	value -= align_value(opponent, bs.get(2), bs.get(4), bs.get(6));
	return value;
}

auto heuristic::better_heuristic(const GameState& game_state, square::type player) -> return_t {

	square::type opponent = GameState::opposite_player(player);
	return_t value{improved_heuristic(game_state, player)};

	auto bs = game_state.get_board_state();
	uint_fast8_t idx{0};
	while(bs.get(idx) != square::type::empty_square) {
		++idx;
	}
	
	square::type idx_m6{idx >= 6 ? bs.get(idx - 6) : square::type::size},
				 idx_m3{idx >= 3 ? bs.get(idx - 3) : square::type::size},
				 idx_m2{idx >= 2 ? bs.get(idx - 2) : square::type::size},
				 idx_m1{idx >= 1 ? bs.get(idx - 1) : square::type::size},
				 idx_p1{idx < 17 ? bs.get(idx + 1) : square::type::size},
				 idx_p2{idx < 16 ? bs.get(idx + 2) : square::type::size},
				 idx_p3{idx < 15 ? bs.get(idx + 3) : square::type::size},
				 idx_p6{idx < 12 ? bs.get(idx + 6) : square::type::size};
	
	value += (align_value(player, idx_m1, idx_p3, idx_p1) * 2) / 5;
	value += (align_value(player, idx_m1, idx_m3, idx_p1) * 2) / 5;
	value += (align_value(player, idx_m3, idx_p1, idx_p3) * 2) / 5;
	value += (align_value(player, idx_m3, idx_m1, idx_p3) * 2) / 5;

	value += (align_value(player, idx_m2, idx_m1, idx_p3) * 2) / 5;
	value += (align_value(player, idx_m2, idx_m1, idx_m3) * 2) / 5;
	value += (align_value(player, idx_p3, idx_p1, idx_p2) * 2) / 5;
	value += (align_value(player, idx_m3, idx_p1, idx_p2) * 2) / 5;

	value += (align_value(player, idx_m1, idx_m3, idx_m6) * 2) / 5;
	value += (align_value(player, idx_p1, idx_m3, idx_m6) * 2) / 5;
	value += (align_value(player, idx_p6, idx_p3, idx_p1) * 2) / 5;
	value += (align_value(player, idx_p6, idx_p3, idx_m1) * 2) / 5;
	
	
	
	value -= (align_value(opponent, idx_m1, idx_p3, idx_p1) * 2) / 5;
	value -= (align_value(opponent, idx_m1, idx_m3, idx_p1) * 2) / 5;
	value -= (align_value(opponent, idx_m3, idx_p1, idx_p3) * 2) / 5;
	value -= (align_value(opponent, idx_m3, idx_m1, idx_p3) * 2) / 5;

	value -= (align_value(opponent, idx_m2, idx_m1, idx_p3) * 2) / 5;
	value -= (align_value(opponent, idx_m2, idx_m1, idx_m3) * 2) / 5;
	value -= (align_value(opponent, idx_p3, idx_p1, idx_p2) * 2) / 5;
	value -= (align_value(opponent, idx_m3, idx_p1, idx_p2) * 2) / 5;

	value -= (align_value(opponent, idx_m1, idx_m3, idx_m6) * 2) / 5;
	value -= (align_value(opponent, idx_p1, idx_m3, idx_m6) * 2) / 5;
	value -= (align_value(opponent, idx_p6, idx_p3, idx_p1) * 2) / 5;
	value -= (align_value(opponent, idx_p6, idx_p3, idx_m1) * 2) / 5;

	return value;
}

bool is_there_a_connected_token(const GameState& game_state, square::type type, uint_fast8_t current_x, uint_fast8_t current_y) {
	for (uint_fast8_t j{BOARD_DIMENSION * BOARD_DIMENSION}; j--;) {
		square::type square_type = game_state.get_board_state().get(j);
		if (square_type == type) {
			uint_fast8_t other_x = j % BOARD_DIMENSION, other_y = j / BOARD_DIMENSION;
			uint_fast8_t x_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(current_x - other_x)));
			uint_fast8_t y_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(current_y - other_y)));
			if ((x_diff == 1 || y_diff == 1) && x_diff + y_diff <= 2) {
				return true;
			}
		}
	}
	return false;
}

auto heuristic::default_heuristic(const GameState& game_state, square::type player) -> return_t {
	return_t score = 0;

	square::type winner;
	if (game_state.is_there_a_winner(&winner)) {
		if (winner == player) {
			return 100;
		} else {
			score = -100;
		}
	}

	square::type opponent;
	if (player == square::type::blue) {
		opponent = square::type::red;
	} else {
		opponent = square::type::blue;
	}

	for (uint_fast8_t i{BOARD_DIMENSION * BOARD_DIMENSION}; i--;) {
		square::type square_type = game_state.get_board_state().get(i);
		if (square_type == player) {
			++score;
			if (is_there_a_connected_token(game_state, player, i % BOARD_DIMENSION, i / BOARD_DIMENSION)) {
				++score;
			}
		} else if (square_type == opponent) {
			--score;
			if (is_there_a_connected_token(game_state, opponent, i % BOARD_DIMENSION, i / BOARD_DIMENSION)) {
				++score;
			}
		}
	}

	return score;
}

auto heuristic::win_lose(const GameState& game_state, square::type player) -> return_t {
	square::type winner;
	if (game_state.is_there_a_winner(&winner)) {
		return winner == player ? std::numeric_limits<return_t>::max() - 1 : std::numeric_limits<return_t>::min() + 1;
	}
	return 0;
}

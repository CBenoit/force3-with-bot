#include <cstdint>

#include "logic/heuristic.hpp"
#include "logic/boardstate.hpp"

using heuristic::return_t;

template <return_t Three = 1000,
		  return_t TwoNAvailable = 21,
		  return_t TwoNEmpty = 16,
		  return_t TwoNOther = 11,
		  return_t One = 5,
		  return_t Zero = 0>
static inline heuristic::return_t align_value(square::type player, square::type st0, square::type st1, square::type st2) {
	if (st0 == player) {
		if (st1 == player) {
			if (st2 == player) {
				return Three;
			} else if (st2 == square::type::available) {
				return TwoNAvailable;
			} else if (st2 == square::type::empty_square) {
				return TwoNEmpty;
			} else {
				return TwoNOther;
			}
		} else if (st2 == player) {
			if (st1 == square::type::available) {
				return TwoNAvailable;
			} else if (st1 == square::type::empty_square) {
				return TwoNEmpty;
			} else {
				return TwoNOther;
			}
		} else {
			return One;
		}
	} else if (st1 == player) {
		if (st2 == player) {
			if (st0 == square::type::available) {
				return TwoNAvailable;
			} else if (st0 == square::type::empty_square) {
				return TwoNEmpty;
			} else {
				return TwoNOther;
			}
		} else {
			return One;
		}
	} else if (st2 == player) {
		return One;
	}
	return Zero;
}

return_t heuristic::hard(const GameState& gs, square::type player) {

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

return_t heuristic::legendary(const GameState& game_state, square::type player) {

	square::type opponent = GameState::opposite_player(player);
	return_t value{hard(game_state, player)};

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
	
	value += align_value<120,30,1,7,5,0>(player, idx_m1, idx_p3, idx_p1);
	value += align_value<120,30,1,7,5,0>(player, idx_m1, idx_m3, idx_p1);
	value += align_value<120,30,1,7,5,0>(player, idx_m3, idx_p1, idx_p3);
	value += align_value<120,30,1,7,5,0>(player, idx_m3, idx_m1, idx_p3);

	value += align_value<120,30,1,7,5,0>(player, idx_m2, idx_m1, idx_p3);
	value += align_value<120,30,1,7,5,0>(player, idx_m2, idx_m1, idx_m3);
	value += align_value<120,30,1,7,5,0>(player, idx_p3, idx_p1, idx_p2);
	value += align_value<120,30,1,7,5,0>(player, idx_m3, idx_p1, idx_p2);

	value += align_value<120,30,1,7,5,0>(player, idx_m1, idx_m3, idx_m6);
	value += align_value<120,30,1,7,5,0>(player, idx_p1, idx_m3, idx_m6);
	value += align_value<120,30,1,7,5,0>(player, idx_p6, idx_p3, idx_p1);
	value += align_value<120,30,1,7,5,0>(player, idx_p6, idx_p3, idx_m1);
	
	
	
	value -= align_value<120,30,1,7,5,0>(opponent, idx_m1, idx_p3, idx_p1);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_m1, idx_m3, idx_p1);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_m3, idx_p1, idx_p3);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_m3, idx_m1, idx_p3);

	value -= align_value<120,30,1,7,5,0>(opponent, idx_m2, idx_m1, idx_p3);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_m2, idx_m1, idx_m3);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_p3, idx_p1, idx_p2);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_m3, idx_p1, idx_p2);

	value -= align_value<120,30,1,7,5,0>(opponent, idx_m1, idx_m3, idx_m6);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_p1, idx_m3, idx_m6);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_p6, idx_p3, idx_p1);
	value -= align_value<120,30,1,7,5,0>(opponent, idx_p6, idx_p3, idx_m1);

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

return_t heuristic::normal(const GameState& game_state, square::type player) {
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

return_t heuristic::easy(const GameState& game_state, square::type player) {
	square::type winner;
	if (game_state.is_there_a_winner(&winner)) {
		return winner == player ? 100 : -100;
	}
	return 0;
}

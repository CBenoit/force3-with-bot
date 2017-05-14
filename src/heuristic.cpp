#include <cstdint>

#include "heuristic.hpp"
#include "boardstate.hpp"

heuristic::return_t heuristic::default_heuristic(const GameState& game_state, square::type player) {
	square::type winner;
	if (game_state.is_there_a_winner(&winner)) {
		return winner == player ? std::numeric_limits<return_t>::max() : std::numeric_limits<return_t>::min();
	}

	return_t score = 0;

	square::type opponent;
	if (player == square::type::blue) {
		opponent = square::type::red;
	} else {
		opponent = square::type::blue;
	}

	for (uint_fast8_t i{BOARD_DIMENSION * BOARD_DIMENSION}; i--;) {
		square::type square_type = game_state.get_board_state().get(i);
		if (square_type == player) {
			uint_fast8_t current_x = i % BOARD_DIMENSION, current_y = i / BOARD_DIMENSION;
			for (uint_fast8_t j{BOARD_DIMENSION * BOARD_DIMENSION}; j--;) {
				square::type square_type = game_state.get_board_state().get(j);
				if (square_type == player) {
					uint_fast8_t other_x = j % BOARD_DIMENSION, other_y = j / BOARD_DIMENSION;
					uint_fast8_t x_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(current_x - other_x)));
					uint_fast8_t y_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(current_y - other_y)));
					if ((x_diff == 1 || y_diff == 1) && x_diff + y_diff <= 2) {
						++score;
					}
				}
			}
		} else if (square_type == opponent) {
			uint_fast8_t current_x = i % BOARD_DIMENSION, current_y = i / BOARD_DIMENSION;
			for (uint_fast8_t j{BOARD_DIMENSION * BOARD_DIMENSION}; j--;) {
				square::type square_type = game_state.get_board_state().get(j);
				if (square_type == opponent) {
					uint_fast8_t other_x = j % BOARD_DIMENSION, other_y = j / BOARD_DIMENSION;
					uint_fast8_t x_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(current_x - other_x)));
					uint_fast8_t y_diff = static_cast<uint_fast8_t>(std::abs(static_cast<int_fast8_t>(current_y - other_y)));
					if ((x_diff == 1 || y_diff == 1) && x_diff + y_diff <= 2) {
						--score;
					}
				}
			}
		}
	}

	return score;
}

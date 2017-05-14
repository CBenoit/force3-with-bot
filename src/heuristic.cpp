#include <cstdint>

#include "heuristic.hpp"
#include "boardstate.hpp"

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
	square::type winner;
	if (game_state.is_there_a_winner(&winner)) {
		return winner == player ? std::numeric_limits<return_t>::max() - 1 : std::numeric_limits<return_t>::min() + 1;
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

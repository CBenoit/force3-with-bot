#include <random>
#include <cstdlib>
#include <ctime>

#include "alphabeta.hpp"
#include "move.hpp"

#define ALPHA_BETA_DEPTH_THRESHOLD 7

AlphaBeta::AlphaBeta(heuristic::function_t heuristic, uint_fast8_t depth)
	: m_depth(depth)
	, m_heuristic(heuristic)
{
	std::srand(std::time(0));
}

move::MoveWrapper AlphaBeta::think(GameState game_state) const {
	if (game_state.is_there_a_winner()) {
		return move::MoveWrapper();
	} else {
		std::vector<GameState> neighbours = game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min() + 1;
		std::vector<move::MoveWrapper> best_moves;
		best_moves.reserve(10);
		for (auto& gs : neighbours) {
			heuristic::return_t ret;
			if (m_depth >= ALPHA_BETA_DEPTH_THRESHOLD) { // enable alpha beta pruning if depth is high.
				ret = -negamax(gs, true, m_depth - 1,
							   max, // alpha
							   std::numeric_limits<heuristic::return_t>::max() - 1); // beta
			} else {
				ret = -negamax(gs, true, m_depth - 1);
			}

			if (ret > max) {
				best_moves.clear();
				best_moves.push_back(gs.get_last_move());
				max = ret;
			} else if (ret == max) {
				best_moves.push_back(gs.get_last_move());
			}
		}
		return best_moves[std::rand() % best_moves.size()];
	}
}

// negamax without alpha beta pruning.
heuristic::return_t AlphaBeta::negamax(GameState game_state, bool is_opponent, size_t remaining_depth) const {
	if (remaining_depth && !game_state.is_there_a_winner()) {
		std::vector<GameState> neighbours = game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
		for (auto& gs : neighbours) {
			max = std::max(max, -negamax(std::move(gs), !is_opponent, remaining_depth - 1));
		}
		return max;
	} else { // this is a leaf
		return is_opponent
				? -m_heuristic(game_state, game_state.get_previous_player())
				: m_heuristic(game_state, game_state.get_current_player());
	}
}

// negamax with alpha beta pruning.
heuristic::return_t AlphaBeta::negamax(GameState game_state, bool is_opponent, size_t remaining_depth,
									   heuristic::return_t alpha, heuristic::return_t beta) const {
	if (remaining_depth && !game_state.is_there_a_winner()) {
		std::vector<GameState> neighbours = game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
		for (auto& gs : neighbours) {
			heuristic::return_t ret = -negamax(std::move(gs), !is_opponent, remaining_depth - 1, -beta, -alpha);
			if (ret > max) {
				max = ret;
				if (max > alpha) {
					alpha = max;
					if (alpha > beta) {
						return max; // cut!
					}
				}
			}
		}
		return max;
	} else { // this is a leaf
		return is_opponent
				? -m_heuristic(game_state, game_state.get_previous_player())
				: m_heuristic(game_state, game_state.get_current_player());
	}

	return 0;
}

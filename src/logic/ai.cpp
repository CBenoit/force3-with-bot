#include <random>
#include <cstdlib>
#include <ctime>

#include "ai.hpp"
#include "move.hpp"

#define ALPHA_BETA_DEPTH_THRESHOLD 7

AI::AI(heuristic::function_t heuristic, uint_fast8_t depth)
	: m_depth(depth)
	, m_heuristic(heuristic)
{
	std::srand(std::time(0));
}

move::MoveWrapper AI::think(GameState game_state) const {
	if (game_state.is_there_a_winner()) {
		return move::MoveWrapper();
	} else {
		std::vector<GameState> neighbours = game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min() + 1;
		std::vector<move::MoveWrapper> best_moves;
		best_moves.reserve(10);
		// iterate over neighbours states and collect best ones
		for (const auto& gs : neighbours) {
			heuristic::return_t ret;
			// evaluate the score for this move with negamax.
			if (m_depth >= ALPHA_BETA_DEPTH_THRESHOLD) { // enable alpha beta pruning if depth is high.
				ret = -negamax(gs, true, m_depth - 1u,
							   max, // alpha
							   std::numeric_limits<heuristic::return_t>::max() - 1); // beta
			} else {
				ret = -negamax(gs, true, m_depth - 1u);
			}

			if (ret > max) {
				// new best move found, discard any "best" move found in the past and collect the new one.
				best_moves.clear();
				best_moves.push_back(gs.get_last_move());
				max = ret;
			} else if (ret == max) {
				// one of the best too, collect it.
				best_moves.push_back(gs.get_last_move());
			}
		}
		return best_moves[std::rand() % best_moves.size()];
	}
}

// negamax without alpha beta pruning.
heuristic::return_t AI::negamax(GameState game_state, bool is_opponent, size_t remaining_depth) const {
	square::type winner;
	if (remaining_depth && !game_state.is_there_a_winner(&winner)) {
		// generates neighbours states and determine the score of this node using their score
		std::vector<GameState> neighbours = game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
		for (auto& gs : neighbours) {
			// takes the opposite of the return's value to alternate between min nodes and max ones:
			// taking the maximum of the opposite values is equivalent to taking the minimum of the values.
			max = std::max(max, -negamax(std::move(gs), !is_opponent, remaining_depth - 1));
		}
		return max;
	} else { // this is a leaf
		heuristic::return_t depth_bonus = 0;
		if (remaining_depth != 0) {
			if (winner == (is_opponent ? game_state.get_previous_player() : game_state.get_current_player())) {
				depth_bonus = remaining_depth;
			} else {
				depth_bonus = -remaining_depth;
			}
		}
		/* by adding or removing the remaining depth to the score value, winning states that are the
		   nearest of the root gets a better score while losing states gets a malus (doesn't matter when
		   it is a not winning state: remaining depth is equal to 0 in that case. */
		return is_opponent
				? -(m_heuristic(game_state, game_state.get_previous_player()) + depth_bonus)
				: (m_heuristic(game_state, game_state.get_current_player()) + depth_bonus);
	}
}

// negamax with alpha beta pruning.
heuristic::return_t AI::negamax(GameState game_state, bool is_opponent, size_t remaining_depth,
									   heuristic::return_t alpha, heuristic::return_t beta) const {
	square::type winner;
	if (remaining_depth && !game_state.is_there_a_winner(&winner)) {
		std::vector<GameState> neighbours = game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
		for (auto& gs : neighbours) {
			// each recursive call to negaxmax takes the *opposite* of beta as alpha and the opposite of alpha as beta.
			// (alternates between min nodes and max nodes)
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
		heuristic::return_t depth_bonus = 0;
		if (remaining_depth != 0) {
			if (winner == (is_opponent ? game_state.get_previous_player() : game_state.get_current_player())) {
				depth_bonus = remaining_depth;
			} else {
				depth_bonus = -remaining_depth;
			}
		}
		return is_opponent
				? -(m_heuristic(game_state, game_state.get_previous_player()) + depth_bonus)
				: (m_heuristic(game_state, game_state.get_current_player()) + depth_bonus);
	}
}

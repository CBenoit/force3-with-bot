#include <random>
#include <cstdlib>
#include <ctime>

#include "alphabeta.hpp"
#include "move.hpp"

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
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
		std::vector<move::MoveWrapper> best_moves;
		best_moves.reserve(10);
		for (auto& gs : neighbours) {
			details::Node child{std::move(gs), true};
			heuristic::return_t ret = -negamax(child, m_depth - 1);
			if (ret > max) {
				best_moves.clear();
				best_moves.push_back(child.game_state.get_last_move());
				max = ret;
			} else if (ret == max) {
				best_moves.push_back(child.game_state.get_last_move());
			}
		}
		return best_moves[std::rand() % best_moves.size()];
	}
}

// negamax without alpha beta pruning.
heuristic::return_t AlphaBeta::negamax(details::Node node, size_t remaining_depth) const {
	if (remaining_depth && !node.game_state.is_there_a_winner()) {
		std::vector<GameState> neighbours = node.game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
		for (auto& gs : neighbours) {
			max = std::max(max, -negamax({std::move(gs), !node.is_opponent}, remaining_depth - 1));
		}
		return max;
	} else {
		return node.is_opponent
				? -m_heuristic(node.game_state, node.game_state.get_previous_player())
				: m_heuristic(node.game_state, node.game_state.get_current_player());
	}
}

// negamax with alpha beta pruning.
heuristic::return_t AlphaBeta::negamax(details::Node /*node*/, size_t /*current_depth*/, heuristic::return_t /*alpha*/, heuristic::return_t /*beta*/) const {
	// TODO

	return 0;
}

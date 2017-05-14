#include "alphabeta.hpp"
#include "move.hpp"

AlphaBeta::AlphaBeta(heuristic::function_t heuristic, uint_fast8_t depth)
	: m_depth(depth)
	, m_heuristic(heuristic)
{}

#include <iostream>
move::MoveWrapper AlphaBeta::think(GameState game_state) const {
	if (game_state.is_there_a_winner()) {
		return move::MoveWrapper();
	} else {
		std::vector<GameState> neighbours = game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
		move::MoveWrapper best_move;
		for (auto& gs : neighbours) {
			details::Node child{std::move(gs), true};
			heuristic::return_t ret = -negamax(child, 1);
			if (ret > max) {
				best_move = child.game_state.get_last_move();
				max = ret;
			}
		}
		return std::move(best_move);
	}
}

heuristic::return_t AlphaBeta::negamax(details::Node node, size_t current_depth) const {
	if (current_depth == m_depth || node.game_state.is_there_a_winner()) {
		return node.is_opponent
				? -m_heuristic(node.game_state, node.game_state.get_previous_player())
				: m_heuristic(node.game_state, node.game_state.get_current_player());
	} else {
		std::vector<GameState> neighbours = node.game_state.generate_neighbours();
		heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
		for (auto& gs : neighbours) {
			max = std::max(max, -negamax({std::move(gs), !node.is_opponent}, current_depth + 1));
		}
		return max;
	}
}

heuristic::return_t AlphaBeta::alphabeta(details::Node /*node*/, size_t /*current_depth*/, heuristic::return_t /*alpha*/, heuristic::return_t /*beta*/) const {
	// TODO: alpha beta pruning based on negamax.

	return 0;
}

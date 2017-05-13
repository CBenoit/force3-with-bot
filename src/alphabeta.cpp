#include "alphabeta.hpp"
#include "move.hpp"
#include "movewrapper.hpp"
#include "tree.hpp"

namespace {
// TO FILL WITH USEFUL STUFF
}

AlphaBeta::AlphaBeta(uint_fast8_t depth)
	: m_depth(depth)
{}

AlphaBeta::AlphaBeta(heuristic_function_t heuristic, uint_fast8_t depth)
	: m_depth(depth)
	, m_heuristic(heuristic)
{}

void AlphaBeta::set_heuristic(heuristic_function_t heuristic) {
	m_heuristic = heuristic;
}

// ===== begin think part =====

move::MoveWrapper think(GameState game_state) {
	Tree<GameState> tree;
	tree.insertRoot(std::move(game_state));

	// TODO

	return move::MoveWrapper();
}

// ===== end think part =====

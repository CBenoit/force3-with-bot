#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include "gamestate.hpp"
#include "movewrapper.hpp"
#include "gamesquare.hpp"
#include "heuristic.hpp"

namespace details {

struct Node {
	Node(GameState game_state, bool is_opponent)
		: is_opponent(is_opponent), game_state(game_state)
	{}

	bool is_opponent;
	GameState game_state;
};

}

/**
 * @brief The AlphaBeta class
 *
 * Alpha Beta with nega max algorithm.
 */
class AlphaBeta
{
public:
	AlphaBeta(heuristic::function_t heuristic = heuristic::default_heuristic, uint_fast8_t depth = 4);
	AlphaBeta(const AlphaBeta&) = default;
	AlphaBeta(AlphaBeta&&) = default;
	AlphaBeta& operator=(const AlphaBeta&) = default;
	~AlphaBeta() {}

	move::MoveWrapper think(GameState game_state) const;

private: // helpers
	// without alpha beta pruning.
	heuristic::return_t negamax(details::Node node, size_t current_depth) const;
	// with alpha beta pruning.
	heuristic::return_t alphabeta(details::Node node, size_t current_depth, heuristic::return_t alpha, heuristic::return_t beta) const;

private:
	uint_fast8_t m_depth;
	heuristic::function_t m_heuristic;
};

#endif // ALPHABETA_HPP

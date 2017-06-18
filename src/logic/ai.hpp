#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include "gamestate.hpp"
#include "movewrapper.hpp"
#include "ui/gamesquare.hpp"
#include "heuristic.hpp"

/**
 * @brief The AI class
 *
 * AI with plain negamax and negamax with alpha beta pruning.
 */
class AI
{
public:
	AI(heuristic::function_t heuristic = &heuristic::normal, uint_fast8_t depth = 7);
	AI(const AI&) = default;
	AI(AI&&) = default;
	AI& operator=(const AI&) = default;
	~AI() {}

	/**
	 * @brief decide a best move using the AI's heuristic and the given game state by running a minmax algorithm.
	 * @param game_state: the game state to think about.
	 * @return the decided move to play.
	 */
	move::MoveWrapper think(GameState game_state) const;

private: // helpers
	// negamax (minmax variant) without alpha beta pruning.
	heuristic::return_t negamax(GameState game_state, bool is_opponent, size_t remaining_depth) const;
	// negamax with alpha beta pruning.
	heuristic::return_t negamax(GameState game_state, bool is_opponent, size_t remaining_depth,
								heuristic::return_t alpha, heuristic::return_t beta) const;

private:
	uint_fast8_t m_depth;
	heuristic::function_t m_heuristic;
};

#endif // ALPHABETA_HPP

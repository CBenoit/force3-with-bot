#ifndef ALPHABETA_HPP
#define ALPHABETA_HPP

#include <cstdint>
#include <functional>

#include "gamestate.hpp"

using heuristic_function_t = std::function<unsigned int(const GameState&)>;

using namespace move; // to forward declare MoveWrapper

class AlphaBeta
{
	// forward declaration
	class MoveWrapper;

public:
	explicit AlphaBeta(uint_fast8_t depth = 3);
	AlphaBeta(heuristic_function_t heuristic, uint_fast8_t depth = 3);
	AlphaBeta(const AlphaBeta&) = default;
	AlphaBeta(AlphaBeta&&) = default;
	AlphaBeta& operator=(const AlphaBeta&) = default;
	~AlphaBeta() {}

	void set_heuristic(heuristic_function_t heuristic);

	MoveWrapper think(const GameState& game_state) const;

private:
	uint_fast8_t m_depth;
	heuristic_function_t m_heuristic;
};

#endif // ALPHABETA_HPP

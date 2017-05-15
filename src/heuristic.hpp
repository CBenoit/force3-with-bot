#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <functional>

#include "gamestate.hpp"
#include "gamesquare.hpp"

namespace heuristic {

using return_t = int;
using function_t = std::function<return_t(const GameState&, square::type)>;

return_t default_heuristic(const GameState&, square::type);

return_t improved_heuristic(const GameState& game_state, square::type player);

return_t better_heuristic(const GameState& game_state, square::type player);

return_t win_lose(const GameState&, square::type);

}

#endif // HEURISTIC_HPP

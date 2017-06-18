#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <functional>

#include "logic/gamestate.hpp"
#include "ui/gamesquare.hpp"

namespace heuristic {

using return_t = int;
using function_t = std::function<return_t(const GameState&, square::type)>;

return_t normal(const GameState&, square::type);

return_t hard(const GameState& game_state, square::type player);

return_t legendary(const GameState& game_state, square::type player);

return_t easy(const GameState&, square::type);

}

#endif // HEURISTIC_HPP

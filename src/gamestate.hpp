#ifndef FORCE3_GAMEMANAGER_HPP
#define FORCE3_GAMEMANAGER_HPP

#include <vector>
#include <cstdint>

#include "gameboardstate.hpp"
#include "gamesquare.hpp"

// forward declaration
namespace move {
struct Slide;
struct Swap;
struct SetColor;
}

class GameState
{
public:
	explicit GameState();
	GameState(const GameState& game_manager);
	GameState& operator=(const GameState& game_manager);
	~GameState();

	template <typename Move>
	bool play(const Move& move) {
		if (impl_is_valid_move(move)) {
			impl_play(move);
			impl_next_turn();
			return true;
		}
		return false;
	}

	square::type get_current_player() const {
		return m_current_player;
	}

	square::type get_previous_player() const {
		if (m_current_player == square::type::blue) {
			return square::type::red;
		}
		return square::type::blue;
	}

	const GameboardState& get_gameboard_state() const {
		return m_gameboard_state;
	}

	std::vector<GameState> generate_neighbours() const;

private:
	void impl_play(const move::Slide& slide);
	void impl_play(const move::Swap& swp);
	void impl_play(const move::SetColor& set_color);

	bool impl_is_valid_move(const move::Slide& slide) const;
	bool impl_is_valid_move(const move::Swap& swp) const;
	bool impl_is_valid_move(const move::SetColor& set_color) const;

	void impl_next_turn();

	// helpers

	bool has_remaining_tokens(square::type square_type) const;

	void decrement_remaining_tokens(square::type square_type);

	void swap(int x1, int y1, int x2, int y2);

	// attributes

	uint_fast8_t m_remaining_blue_tokens;
	uint_fast8_t m_remaining_red_tokens;

	square::type m_current_player;
	GameboardState m_gameboard_state;

	template <typename Move>
	friend GameState generate_game_state_from_move(const GameState& current_game_state, const Move& move);
};

#endif // FORCE3_GAMEMANAGER_HPP

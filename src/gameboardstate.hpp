#ifndef GAMEBOARDSTATE_HPP
#define GAMEBOARDSTATE_HPP

#include <cstdint>
#include <array>
#include <ostream>
#include <vector>

#include "boardsquare.hpp"

class GameboardState {

public:
	GameboardState() : m_board{0} {}
	GameboardState(const GameboardState&) = default;
	explicit GameboardState(std::array<std::array<square::type,3>,3> values);

	explicit GameboardState(std::initializer_list<square::type> il);

	explicit GameboardState(std::initializer_list<std::initializer_list<square::type>> il);

	explicit GameboardState(const square::type values[3][3]);

	explicit GameboardState(uint_fast32_t board) : m_board(board) {}

	GameboardState& operator=(const GameboardState&) = default;
	~GameboardState() {}

	bool operator==(const GameboardState& other) const;

	square::type get(unsigned int x, unsigned int y) const;
	/**
	 * get(x, y) == get(x + 3*y)
	 */
	square::type get(unsigned int idx) const;

	void set(unsigned int x, unsigned int y, square::type value);

	/**
	 * set(x, y, v) <=> set(x + 3*y, v)
	 */
	void set(unsigned int idx, square::type value);

	/**
	 * Finds and returns the list of lines that are not equal in (*this) and (other).
	 */
	std::vector<unsigned int> line_diff(const GameboardState& other) const;

private:
	uint_fast32_t m_board;
};

std::ostream& operator<<(std::ostream& os, const GameboardState& gs);

#endif // GAMEBOARDSTATE_HPP

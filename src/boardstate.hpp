#ifndef GAMEBOARDSTATE_HPP
#define GAMEBOARDSTATE_HPP

#include <cstdint>
#include <array>
#include <ostream>
#include <vector>

#include "boardsquare.hpp"

#define BOARD_DIMENSION 3

class BoardState {

public:
	BoardState() : m_board{0} {}
	BoardState(const BoardState&) = default;
	explicit BoardState(std::array<std::array<square::type,3>,3> values);

	explicit BoardState(std::initializer_list<square::type> il);

	explicit BoardState(std::initializer_list<std::initializer_list<square::type>> il);

	explicit BoardState(const square::type values[3][3]);

	explicit BoardState(uint_fast32_t board) : m_board(board) {}

	BoardState& operator=(const BoardState&) = default;
	~BoardState() {}

	bool operator==(const BoardState& other) const;

	square::type get(unsigned int x, unsigned int y) const;

	/**
	 * get(x, y) == get(x + 3*y)
	 */
	square::type get(unsigned int idx) const {
		return static_cast<square::type>((m_board >> (2 * idx)) & 3);
	}

	void set(unsigned int x, unsigned int y, square::type value);

	/**
	 * set(x, y, v) <=> set(x + 3*y, v)
	 */
	void set(unsigned int idx, square::type value);

	/**
	 * Finds and returns the list of lines that are not equal in (*this) and (other).
	 */
	std::vector<unsigned int> line_diff(const BoardState& other) const;

private:
	uint_fast32_t m_board;
};

std::ostream& operator<<(std::ostream& os, const BoardState& gs);

#endif // GAMEBOARDSTATE_HPP

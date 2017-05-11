#include "gameboardstate.hpp"

#include <vector>

GameboardState::GameboardState(std::array<std::array<square::type,3>,3> values) : m_board{0} {
	for (uint_fast8_t i{3} ; i-- ;) {
		for (uint_fast8_t j{3} ; j-- ;) {
			set(i, j, values[i][j]);
		}
	}
}

GameboardState::GameboardState(std::initializer_list<square::type> il) : m_board{0} {
	unsigned int i{0};
	for (auto it = il.begin(), end = il.end() ; it != end ; ++it, ++i) {
		set(i, *it);
	}
}

GameboardState::GameboardState(std::initializer_list<std::initializer_list<square::type>> il) : m_board{0} {
	unsigned int i{0};
	for (auto it = il.begin(), end = il.end() ; it != end ; ++it) {
		for (auto it2 = it->begin(), end2 = it->end() ; it2 != end2 ; ++it2, ++i) {
			set(i, *it2);
		}
	}
}

GameboardState::GameboardState(const square::type values[3][3]) : m_board{0} {
	for (uint_fast8_t i{3} ; i-- ;) {
		for (uint_fast8_t j{3} ; j-- ;) {
			set(i, j, values[i][j]);
		}
	}
}

inline bool GameboardState::operator==(const GameboardState& other) const {
	return m_board == other.m_board;
}

inline square::type GameboardState::get(unsigned int x, unsigned int y) const {
	return static_cast<square::type>((m_board >> (6 * y + 2 * x)) & 3);
}

/**
 * get(x, y) == get(x + 3*y)
 */
inline square::type GameboardState::get(unsigned int idx) const {
	return static_cast<square::type>((m_board >> (2 * idx)) & 3);
}

inline void GameboardState::set(unsigned int x, unsigned int y, square::type value) {
	uint_fast8_t r_idx = static_cast<uint_fast8_t>(6 * y + 2 * x);
	m_board = (m_board & ~(3 << r_idx)) | (static_cast<uint_fast8_t>(value) << r_idx);
}

/**
 * set(x, y, v) <=> set(x + 3*y, v)
 */
inline void GameboardState::set(unsigned int idx, square::type value) {
	idx <<= 1;
	m_board = (m_board & ~(3 << idx)) | (static_cast<uint_fast8_t>(value) << idx);
}

/**
 * Finds and returns the list of lines that are not equal in (*this) and (other).
 */
inline std::vector<unsigned int> GameboardState::line_diff(const GameboardState& other) const {
	std::vector<unsigned int> ans;
	for (uint_fast8_t i{3} ; i-- ;) {
		if (((m_board >> (6*i)) & 63) != ((other.m_board >> (6*i)) & 63))
			ans.push_back(i);
	}
	return ans;
}

std::ostream& operator<<(std::ostream& os, const GameboardState& gs) {
	for (uint_fast8_t i{0} ; i < 3 ; i++) {
		for (uint_fast8_t j{0} ; j < 3; j++) {
			switch(gs.get(j,i)) {
			case square::type::available:
				os << '#';
				break;
			case square::type::empty_square:
				os << ' ';
				break;
			case square::type::red:
				os << 'R';
				break;
			case square::type::blue:
				os << 'B';
				break;
			default:
				os << '?';
				break;
			}
		}
		os << '\n';
	}
	return os;
}

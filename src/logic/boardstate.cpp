///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "boardstate.hpp"
#include "movewrapper.hpp"

#include <vector>

BoardState::BoardState(std::array<std::array<square::type,3>,3> values) : m_board{0} {
	for (uint_fast8_t i{3} ; i-- ;) {
		for (uint_fast8_t j{3} ; j-- ;) {
			set(i, j, values[i][j]);
		}
	}
}

BoardState::BoardState(std::initializer_list<square::type> il) : m_board{0} {
	unsigned int i{0};
	for (auto it = il.begin(), end = il.end() ; it != end ; ++it, ++i) {
		set(i, *it);
	}
}

BoardState::BoardState(std::initializer_list<std::initializer_list<square::type>> il) : m_board{0} {
	unsigned int i{0};
	for (auto it = il.begin(), end = il.end() ; it != end ; ++it) {
		for (auto it2 = it->begin(), end2 = it->end() ; it2 != end2 ; ++it2, ++i) {
			set(i, *it2);
		}
	}
}

BoardState::BoardState(const square::type values[3][3]) : m_board{0} {
	for (uint_fast8_t i{3} ; i-- ;) {
		for (uint_fast8_t j{3} ; j-- ;) {
			set(i, j, values[i][j]);
		}
	}
}

/**
 * Finds and returns the list of lines that are not equal in (*this) and (other).
 */
inline std::vector<unsigned int> BoardState::line_diff(const BoardState& other) const {
	std::vector<unsigned int> ans;
	for (uint_fast8_t i{3} ; i-- ;) {
		if (((m_board >> (6*i)) & 63) != ((other.m_board >> (6*i)) & 63))
			ans.push_back(i);
	}
	return ans;
}

std::ostream& operator<<(std::ostream& os, const BoardState& gs) {
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

move::MoveWrapper diff(BoardState prev, BoardState next) {

	uint_fast8_t i{0};
	uint_fast8_t arr[3][3];
	square::type pst, nst;
	for (uint_fast8_t idx{9} ; idx--;) {
		pst = prev.get(idx);
		nst = next.get(idx);
		if (pst != nst) {
			arr[0][i  ] = idx;
			arr[1][i  ] = static_cast<uint_fast8_t>(pst);
			arr[2][i++] = static_cast<uint_fast8_t>(nst);
		}
	}

	--i;
	if (i == 0) {
		return move::MoveWrapper(move::SetColor(arr[0][0] % 3, arr[0][0] / 3));
	}

	if (arr[1][0] == static_cast<uint_fast8_t>(square::type::empty_square)) {
		return move::MoveWrapper(move::Slide(arr[0][0] % 3, arr[0][0] / 3, arr[0][i] % 3, arr[0][i] / 3));
	}

	if (arr[2][0] == static_cast<uint_fast8_t>(square::type::empty_square)) {
		return move::MoveWrapper(move::Slide(arr[0][i] % 3, arr[0][i] / 3, arr[0][0] % 3, arr[0][0] / 3));
	}

	return move::MoveWrapper(move::Swap(arr[0][0] % 3, arr[0][0] / 3, arr[0][i] % 3, arr[0][i] / 3));
}

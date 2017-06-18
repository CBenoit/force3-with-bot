///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

namespace move {

struct Slide {
	Slide(uint_fast8_t from_x, uint_fast8_t from_y, uint_fast8_t to_x, uint_fast8_t to_y)
		: from_x(from_x), from_y(from_y), to_x(to_x), to_y(to_y)
	{}

	uint_fast8_t from_x;
	uint_fast8_t from_y;
	uint_fast8_t to_x;
	uint_fast8_t to_y;
};

struct Swap {
	Swap(uint_fast8_t x1, uint_fast8_t y1, uint_fast8_t x2, uint_fast8_t y2)
		: from_x(x1), from_y(y1), to_x(x2), to_y(y2)
	{}

	uint_fast8_t from_x;
	uint_fast8_t from_y;
	uint_fast8_t to_x;
	uint_fast8_t to_y;
};

struct SetColor {
	SetColor(uint_fast8_t x, uint_fast8_t y) : x(x), y(y) {}

	uint_fast8_t x;
	uint_fast8_t y;
};

}

#endif // MOVE_HPP

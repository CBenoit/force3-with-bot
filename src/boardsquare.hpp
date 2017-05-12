///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BOARDSQUARE_HPP
#define BOARDSQUARE_HPP


#define AS_COLORS(x,y) x,
#define AS_TYPE(x,y) y,
#define AS_EXT_TYPE(x,y) square::type::y,
#define COLORS_AND_PLAYERS(ENTRY) \
  ENTRY(Qt::GlobalColor::black, empty_square) \
  ENTRY(Qt::GlobalColor::lightGray, available) \
  ENTRY(Qt::GlobalColor::red, red) \
  ENTRY(Qt::GlobalColor::blue, blue)

namespace square {

	enum class type : unsigned char {
		COLORS_AND_PLAYERS(AS_TYPE)
		size
	};
}

#endif // SQUARE_HPP

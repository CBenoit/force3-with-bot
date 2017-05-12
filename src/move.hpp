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

namespace move {

struct Slide {
	Slide(int from_x, int from_y, int to_x, int to_y)
		: from_x(from_x), from_y(from_y), to_x(to_x), to_y(to_y)
	{}

	int from_x;
	int from_y;
	int to_x;
	int to_y;
};

struct Swap {
	Swap(int x1, int y1, int x2, int y2)
		: x1(x1), y1(y1), x2(x2), y2(y2)
	{}

	int x1;
	int y1;
	int x2;
	int y2;
};

struct SetColor {
	SetColor(int x, int y) : x(x), y(y) {}

	int x;
	int y;
};

}

#endif // MOVE_HPP

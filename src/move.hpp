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

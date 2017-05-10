#ifndef BOARDSQUARE_HPP
#define BOARDSQUARE_HPP


#define AS_COLORS(x,y) x,
#define AS_TYPE(x,y) y,
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

#ifndef ALPHABETAMOVE_HPP
#define ALPHABETAMOVE_HPP

#include "common/move.hpp"

#define MOVE_TYPE_NONE 0
#define MOVE_TYPE_SLIDE 1
#define MOVE_TYPE_SWAP 2
#define MOVE_TYPE_SET_COLOR 3

namespace details { // private api

union Move {
	Move() {}

	Move(move::Slide slide)
		: slide{slide}
	{}

	Move(move::Swap swap)
		: swap{swap}
	{}

	Move(move::SetColor set_color)
		: set_color{set_color}
	{}

	move::Slide slide;
	move::Swap swap;
	move::SetColor set_color;
};

}

namespace move {

class MoveWrapper {
public:
	MoveWrapper()
		: type(MOVE_TYPE_NONE)
	{}

	MoveWrapper(move::Slide slide)
		: type{MOVE_TYPE_SLIDE}
		, move{slide}
	{}

	MoveWrapper(move::Swap swap)
		: type{MOVE_TYPE_SWAP}
		, move{swap}
	{}

	MoveWrapper(move::SetColor set_color)
		: type{MOVE_TYPE_SET_COLOR}
		, move{set_color}
	{}

	void set_none() { type = MOVE_TYPE_NONE; }

	void set_move(move::Slide slide) {
		type = MOVE_TYPE_SLIDE;
		move.slide = slide;
	}

	void set_move(move::Swap swap) {
		type = MOVE_TYPE_SWAP;
		move.swap = swap;
	}

	void set_move(move::SetColor set_color) {
		type = MOVE_TYPE_SET_COLOR;
		move.set_color = set_color;
	}

	bool is_none() const { return type == MOVE_TYPE_NONE; }
	bool is_slide() const { return type == MOVE_TYPE_SLIDE; }
	bool is_swap() const { return type == MOVE_TYPE_SWAP; }
	bool is_set_color() const { return type == MOVE_TYPE_SET_COLOR; }

	move::Slide unwrap_slide() const { return move.slide; }
	move::Swap unwrap_swap() const { return move.swap; }
	move::SetColor unwrap_set_color() const { return move.set_color; }

private:
	uint_fast8_t type;
	details::Move move;
};

}

#endif // ALPHABETAMOVE_HPP

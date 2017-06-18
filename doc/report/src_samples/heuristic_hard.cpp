return_t heuristic::hard(const GameState& gs, square::type player) {
	square::type opponent = GameState::opposite_player(player);
	return_t value{0};

	BoardState bs{gs.get_board_state()};
	for (uint_fast8_t i{BOARD_DIMENSION} ; i-- ;) {
		square::type vi_0 = bs.get(i), vi_1 = bs.get(i + 3), vi_2 = bs.get(i + 6);
		square::type v0_i = bs.get(i*3), v1_i = bs.get(1,i), v2_i = bs.get(2,i);

		// vertical
		value += align_value(player, vi_0, vi_1, vi_2);
		value -= align_value(opponent, vi_0, vi_1, vi_2);

		// horizontal
		value += align_value(player, v0_i,  v1_i, v2_i);
		value -= align_value(opponent, v0_i,  v1_i, v2_i);
	}

	// diagonals
	value += align_value(player, bs.get(0), bs.get(4), bs.get(8));
	value += align_value(player, bs.get(2), bs.get(4), bs.get(6));
	value -= align_value(opponent, bs.get(0), bs.get(4), bs.get(8));
	value -= align_value(opponent, bs.get(2), bs.get(4), bs.get(6));
	return value;
}

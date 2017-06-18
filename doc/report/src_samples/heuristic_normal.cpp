return_t heuristic::normal(const GameState& game_state, square::type player,
                            size_t depth) {
    return_t score = depth;

    square::type winner;
    if (game_state.is_there_a_winner(&winner)) {
        if (winner == player) {
            return 100 - depth;
        } else {
            score = -100;
        }
    }

    square::type opponent;
    if (player == square::type::blue) {
        opponent = square::type::red;
    } else {
        opponent = square::type::blue;
    }

    for (uint_fast8_t i{BOARD_DIMENSION * BOARD_DIMENSION}; i--;) {
        square::type square_type = game_state.get_board_state().get(i);
        if (square_type == player) {
            ++score;
            if (is_there_a_connected_token(game_state, player,
                                            i % BOARD_DIMENSION,
                                            i / BOARD_DIMENSION)) {
                ++score;
            }
        } else if (square_type == opponent) {
            --score;
            if (is_there_a_connected_token(game_state, opponent,
                                            i % BOARD_DIMENSION,
                                            i / BOARD_DIMENSION)) {
                ++score;
            }
        }
    }

    return score;
}


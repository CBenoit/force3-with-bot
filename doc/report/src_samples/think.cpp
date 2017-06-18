move::MoveWrapper AI::think(GameState game_state) const {
    if (game_state.is_there_a_winner()) {
        return move::MoveWrapper();
    } else {
        std::vector<GameState> neighbours = game_state.generate_neighbours();
        heuristic::return_t max =
            std::numeric_limits<heuristic::return_t>::min() + 1;
        std::vector<move::MoveWrapper> best_moves;
        best_moves.reserve(10);
        // iterate over neighbours states and collect best ones
        for (const auto& gs : neighbours) {
            heuristic::return_t ret;
            // evaluate the score for this move with negamax.
            if (m_depth >= ALPHA_BETA_DEPTH_THRESHOLD) {
                // enable alpha beta pruning if depth is high.
                ret = -negamax(gs, true, m_depth - 1,
                    max, // alpha
                    std::numeric_limits<heuristic::return_t>::max() - 1); // beta
            } else {
                ret = -negamax(gs, true, m_depth - 1);
            }

            if (ret > max) {
                // new best move found, discard any "best" move found in the
                // past and collect the new one.
                best_moves.clear();
                best_moves.push_back(gs.get_last_move());
                max = ret;
            } else if (ret == max) {
                // one of the best too, collect it.
                best_moves.push_back(gs.get_last_move());
            }
        }
        return best_moves[std::rand() % best_moves.size()];
    }
}

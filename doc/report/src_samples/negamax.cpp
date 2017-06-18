// negamax with alpha beta pruning.
heuristic::return_t AI::negamax(GameState game_state, bool is_opponent,
                                size_t remaining_depth,
                                heuristic::return_t alpha,
                                heuristic::return_t beta) const {
    if (remaining_depth && !game_state.is_there_a_winner()) {
        std::vector<GameState> neighbours = game_state.generate_neighbours();
        heuristic::return_t max = std::numeric_limits<heuristic::return_t>::min();
        for (auto& gs : neighbours) {
            // takes the opposite of the return's value to alternate
            // between min nodes and max ones:
            // taking the maximum of the opposite values is equivalent
            // to taking the minimum of the values.
            // each recursive call to negaxmax takes the *opposite*
            // of beta as alpha and the opposite of alpha as beta.
            // (alternates between min nodes and max nodes)
            heuristic::return_t ret = -negamax(std::move(gs), !is_opponent,
                                                remaining_depth - 1,
                                                -beta, -alpha);
            if (ret > max) {
                max = ret;
                if (max > alpha) {
                    alpha = max;
                    if (alpha > beta) {
                        return max; // cut!
                    }
                }
            }
        }
        return max;
    } else { // this is a leaf
        return is_opponent
            ? -(m_heuristic(game_state,
                        game_state.get_previous_player()) + remaining_depth)
            : (m_heuristic(game_state,
                        game_state.get_current_player()) + remaining_depth);
        // by adding the remaining depth to the score value, winning states
        // that are the nearest of the root gets a better score (doesn't
        // matter when it is a not winning state: remaining depth is equal
        // to 0 in that case).
    }
}

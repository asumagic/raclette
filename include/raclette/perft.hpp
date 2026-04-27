#pragma once

#include <raclette/movegen.hpp>
#include <raclette/position.hpp>

namespace raclette {

int perft(Position& position, int depth, bool dump_this_level = false) {
	if (depth == 0) {
		return 1;
	}

	MoveGenerator gen{position};

	int moves = 0;

	raclette::Move m;
	while ((m = gen.next(), m.is_valid())) {
		Position next_position{position};
		next_position.play_move(m);

		const auto last_moves = perft(next_position, depth - 1, false);

		if (dump_this_level) {
			fmt::println(
			    "{}{} - {}",
			    next_position.as_global_pos(m.from, true),
			    next_position.as_global_pos(m.to, true),
			    last_moves
			);
		}

		moves += last_moves;
	}

	return moves;
}

} // namespace raclette
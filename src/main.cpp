#include "raclette/location.hpp"
#include <fmt/base.h>
#include <raclette/debug.hpp>
#include <raclette/movegen.hpp>
#include <raclette/parser/fen.hpp>
#include <raclette/position.hpp>

int main() {
	raclette::Position b;
	// set_initial_position(b);

	raclette::parser::parse_fen_position(b, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

	raclette::MoveGenerator        generator(b);
	const raclette::GlobalLocation pos_to_test{7, 1};
	generator.seek(pos_to_test);

	raclette::Move m;
	while ((m = generator.next(), m.is_valid())) {
		fmt::println("{} -> {}", m.from, m.to);
	}

	print_board_pretty(b);
}
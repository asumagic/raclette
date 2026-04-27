#include <fmt/base.h>
#include <raclette/debug.hpp>
#include <raclette/movegen.hpp>
#include <raclette/parser/fen.hpp>
#include <raclette/position.hpp>
#include <string_view>

int main(int argc, char** argv) {
	if (argc < 3) {
		fmt::print(
		    "usage: raclette [action] [params]\n"
		    "Available actions:\n"
		    "	enum [FEN]\n"
		);
		return 1;
	}

	const std::string_view action = argv[1];

	if (action == "enum") {
		raclette::Position b;
		raclette::parser::parse_fen_position(b, argv[2]);

		raclette::MoveGenerator generator(b);
		// const raclette::GlobalLocation pos_to_test{0, 0};

		raclette::Move m;
		while ((m = generator.next(), m.is_valid())) {
			fmt::println("{}{}", m.from, m.to);
		}

		print_board_pretty(b);
	} else {
		fmt::println("Unknown action passed: {}", action);
		return 1;
	}
}
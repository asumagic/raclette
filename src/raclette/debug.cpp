#include <raclette/debug.hpp>
#include <raclette/position.hpp>

#include <fmt/core.h>

namespace raclette {

void print_piece_pretty(const Position& position, Cell cell) {
	const auto loc = position.as_global_pos(cell.loc, true);

	if ((loc.x + loc.y) % 2 == 0) {
		fmt::print("\e[0;100m");
	} else {
		fmt::print("\e[0;107m");
	}
	if (!cell.is_ours) {
		switch (cell.p) {
		case PieceKind::NONE:   fmt::print(" "); break;
		case PieceKind::KING:   fmt::print("♚"); break;
		case PieceKind::QUEEN:  fmt::print("♛"); break;
		case PieceKind::ROOK:   fmt::print("♜"); break;
		case PieceKind::BISHOP: fmt::print("♝"); break;
		case PieceKind::KNIGHT: fmt::print("♞"); break;
		case PieceKind::PAWN:   fmt::print("♟"); break;
		}
	} else {
		switch (cell.p) {
		case PieceKind::NONE:   fmt::print(" "); break;
		case PieceKind::KING:   fmt::print("♔"); break;
		case PieceKind::QUEEN:  fmt::print("♕"); break;
		case PieceKind::ROOK:   fmt::print("♖"); break;
		case PieceKind::BISHOP: fmt::print("♗"); break;
		case PieceKind::KNIGHT: fmt::print("♘"); break;
		case PieceKind::PAWN:   fmt::print("♙"); break;
		}
	}
	fmt::print(" \e[0m");
}

void print_board_pretty(const Position& position, const std::vector<Move>& highlighted_moves) {
	fmt::println("  a b c d e f g h");
	for (int y = 7; y >= 0; --y) {
		fmt::print("{} ", y + 1);
		for (int x = 0; x < 8; ++x) {
			print_piece_pretty(position, position.get_unsafe({x, y}));
		}
		fmt::println(" {}", y + 1);
	}
	fmt::println("  a b c d e f g h");
}

} // namespace raclette
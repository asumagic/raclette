#include <raclette/position.hpp>

#include <tuple>

namespace raclette {

void set_initial_position(Position& position) {
	static constexpr std::array<std::tuple<PieceKind, int, int>, 16> initial_positions{{
	    {PieceKind::ROOK, 0, 0},
	    {PieceKind::KNIGHT, 1, 0},
	    {PieceKind::BISHOP, 2, 0},
	    {PieceKind::QUEEN, 3, 0},
	    {PieceKind::KING, 4, 0},
	    {PieceKind::BISHOP, 5, 0},
	    {PieceKind::KNIGHT, 6, 0},
	    {PieceKind::ROOK, 7, 0},
	    {PieceKind::PAWN, 0, 1},
	    {PieceKind::PAWN, 1, 1},
	    {PieceKind::PAWN, 2, 1},
	    {PieceKind::PAWN, 3, 1},
	    {PieceKind::PAWN, 4, 1},
	    {PieceKind::PAWN, 5, 1},
	    {PieceKind::PAWN, 6, 1},
	    {PieceKind::PAWN, 7, 1},
	}};

	for (const auto [piece, x, y] : initial_positions) {
		position.set({x, y}, piece, true);
		position.set({x, 7 - y}, piece, false);
	}
}

} // namespace raclette
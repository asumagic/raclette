#include <raclette/position.hpp>

#include <tuple>

namespace raclette {

void set_initial_position(Position& position) {
	static constexpr std::array<std::tuple<Piece, int, int>, 16> initial_positions{{
	    {Piece::ROOK, 0, 0},
	    {Piece::KNIGHT, 1, 0},
	    {Piece::BISHOP, 2, 0},
	    {Piece::QUEEN, 3, 0},
	    {Piece::KING, 4, 0},
	    {Piece::BISHOP, 5, 0},
	    {Piece::KNIGHT, 6, 0},
	    {Piece::ROOK, 7, 0},
	    {Piece::PAWN, 0, 1},
	    {Piece::PAWN, 1, 1},
	    {Piece::PAWN, 2, 1},
	    {Piece::PAWN, 3, 1},
	    {Piece::PAWN, 4, 1},
	    {Piece::PAWN, 5, 1},
	    {Piece::PAWN, 6, 1},
	    {Piece::PAWN, 7, 1},
	}};

	for (const auto [piece, x, y] : initial_positions) {
		position.set({x, y}, piece, true);
		position.set({x, 7 - y}, piece, false);
	}
}

} // namespace raclette
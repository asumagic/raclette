#include <raclette/parser/fen.hpp>

namespace raclette::parser {

void parse_fen_position(Position& position, std::string_view fen) {
	int        x = 0, y = 7;
	const bool black_is_us = false, white_is_us = true;
	for (char c : fen) {
		switch (c) {
		case 'r':
			position.set({x, y}, Piece::ROOK, black_is_us);
			++x;
			break;
		case 'n':
			position.set({x, y}, Piece::KNIGHT, black_is_us);
			++x;
			break;
		case 'b':
			position.set({x, y}, Piece::BISHOP, black_is_us);
			++x;
			break;
		case 'q':
			position.set({x, y}, Piece::QUEEN, black_is_us);
			++x;
			break;
		case 'k':
			position.set({x, y}, Piece::KING, black_is_us);
			++x;
			break;
		case 'p':
			position.set({x, y}, Piece::PAWN, black_is_us);
			++x;
			break;

		case 'R':
			position.set({x, y}, Piece::ROOK, white_is_us);
			++x;
			break;
		case 'N':
			position.set({x, y}, Piece::KNIGHT, white_is_us);
			++x;
			break;
		case 'B':
			position.set({x, y}, Piece::BISHOP, white_is_us);
			++x;
			break;
		case 'Q':
			position.set({x, y}, Piece::QUEEN, white_is_us);
			++x;
			break;
		case 'K':
			position.set({x, y}, Piece::KING, white_is_us);
			++x;
			break;
		case 'P':
			position.set({x, y}, Piece::PAWN, white_is_us);
			++x;
			break;

		case '/':
			--y;
			x = 0;
			break;

		case ' ': return;
		}
	}
}

} // namespace raclette::parser
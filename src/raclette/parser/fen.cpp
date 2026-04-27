#include <raclette/parser/fen.hpp>

namespace raclette::parser {

enum class FenParserState { PARSE_PIECES, PARSE_COLOR, DONE };

void parse_fen_position(Position& position, std::string_view fen) {
	int        x = 0, y = 7;
	const bool black_is_us = false, white_is_us = true;

	FenParserState state = FenParserState::PARSE_PIECES;
	std::size_t    i     = 0;

	// parse chess pieces
	while (i < fen.size() && state == FenParserState::PARSE_PIECES) {
		char c = fen[i];

		switch (c) {
		case 'r':
			position.set({x, y}, PieceKind::ROOK, black_is_us);
			++x;
			break;
		case 'n':
			position.set({x, y}, PieceKind::KNIGHT, black_is_us);
			++x;
			break;
		case 'b':
			position.set({x, y}, PieceKind::BISHOP, black_is_us);
			++x;
			break;
		case 'q':
			position.set({x, y}, PieceKind::QUEEN, black_is_us);
			++x;
			break;
		case 'k':
			position.set({x, y}, PieceKind::KING, black_is_us);
			++x;
			break;
		case 'p':
			position.set({x, y}, PieceKind::PAWN, black_is_us);
			++x;
			break;

		case 'R':
			position.set({x, y}, PieceKind::ROOK, white_is_us);
			++x;
			break;
		case 'N':
			position.set({x, y}, PieceKind::KNIGHT, white_is_us);
			++x;
			break;
		case 'B':
			position.set({x, y}, PieceKind::BISHOP, white_is_us);
			++x;
			break;
		case 'Q':
			position.set({x, y}, PieceKind::QUEEN, white_is_us);
			++x;
			break;
		case 'K':
			position.set({x, y}, PieceKind::KING, white_is_us);
			++x;
			break;
		case 'P':
			position.set({x, y}, PieceKind::PAWN, white_is_us);
			++x;
			break;

		case '/':
			--y;
			x = 0;
			break;

		case '1': x += 1; break;
		case '2': x += 2; break;
		case '3': x += 3; break;
		case '4': x += 4; break;
		case '5': x += 5; break;
		case '6': x += 6; break;
		case '7': x += 7; break;
		case '8': x += 8; break;

		case ' ': state = FenParserState::PARSE_COLOR; break;
		}

		++i;
	}

	// parse color playing
	position.set_white_playing(fen[i] == 'w');
	i += 2;
	state = FenParserState::DONE;

	// TODO: parse en passant, halfmove clock, fullmove count
}

} // namespace raclette::parser
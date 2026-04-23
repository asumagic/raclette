#include "raclette/location.hpp"
#include <raclette/movegen.hpp>

#include <raclette/move.hpp>
#include <raclette/position.hpp>

#include <cassert>

namespace raclette {

Move MoveGenerator::next() {

	for (;;) {
		const Cell cell = *get_at(_loc);

		if (!cell.is_ours) {
			if (!skip_to_next_cell()) {
				return invalid_move_magic;
			}
			continue;
		}

		switch (cell.cell.p) {
		case Piece::KING:   break;
		case Piece::QUEEN:  break;
		case Piece::ROOK:   break;
		case Piece::BISHOP: break;
		case Piece::KNIGHT: break;
		case Piece::PAWN:   {
			const bool is_starting_position = _loc.y == 1;
			while (_pawn.state != PawnMoveState::DONE) {
				switch (PawnMoveState::GeneratorState(_pawn.state++)) {
					// FIXME: holy hell en passant
				case PawnMoveState::TWO_AHEAD:
					if (is_starting_position && get_at(_loc.offset(0, 1))->is_empty()
					    && get_at(_loc.offset(0, 2))->is_empty()) {
						return a_move_to(_loc.offset(0, 2));
					}
					break;
				case PawnMoveState::ONE_AHEAD:
					if (const auto target = try_simple_move(_loc.offset(0, 1))) {
						return *target;
					}
					break;
				case PawnMoveState::TAKE_LEFT:
					if (const auto target = try_simple_take(_loc.offset(-1, 1))) {
						return *target;
					}
					break;
				case PawnMoveState::TAKE_RIGHT:
					if (const auto target = try_simple_take(_loc.offset(1, 1))) {
						return *target;
					}
					break;
				case PawnMoveState::DONE: _pawn = {};
				}
			}
		}
		case Piece::NONE: break;
		}

		if (!skip_to_next_cell()) {
			return invalid_move_magic;
		}
	}
}

bool MoveGenerator::skip_to_next_cell() {
	if (_loc == SideRelativeLocation{7, 7}) {
		return false;
	}
	++_loc.x;

	if (_loc.x == 8) {
		_loc.x = 0;
		++_loc.y;
	}

	return true;
}

} // namespace raclette
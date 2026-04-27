#include "raclette/location.hpp"
#include <raclette/movegen.hpp>

#include <raclette/move.hpp>
#include <raclette/position.hpp>

#include <cassert>

namespace raclette {

Move MoveGenerator::next() {
	find_first_iterable();

	// FIXME: if the king is in a check, then the next move should undo that situation
	// FIXME: the next move should not put the king in a check

	const auto try_rays_with_state = [&](auto& state) -> std::optional<Move> {
		const auto next_ray_direction = [&] {
			++state.ray_idx;
			state.ray_dist = 0;
		};

		while (state.ray_idx < state.ray_directions.size()) {
			const auto [ray_dir_x, ray_dir_y] = state.ray_directions[state.ray_idx];

			++state.ray_dist;

			const SideRelativeLocation candidate_pos{
			    _loc.x - (ray_dir_x * state.ray_dist),
			    _loc.y - (ray_dir_y * state.ray_dist)
			};

			if (!candidate_pos.is_in_bounds()) {
				next_ray_direction();
				continue;
			}

			if (const auto target = try_take(candidate_pos)) {
				next_ray_direction(); // can't go beyond that piece
				return target;
			}

			if (const auto target = try_empty_move(candidate_pos)) {
				++state.ray_idx;
				return target;
			}

			next_ray_direction();
		}

		return std::nullopt;
	};

	while (_current != MoveGeneratorState::FINISHED) {
		switch (_current) {
		case MoveGeneratorState::ITER_KING: {
			while (_king.move_idx < _king.valid_offsets.size()) {
				const auto [x, y] = _king.valid_offsets[_king.move_idx++];
				if (const auto target = try_move_or_take(_loc.offset(x, y))) {
					return *target;
				}
			}
			break;
		}

		case MoveGeneratorState::ITER_QUEEN: {
			if (const auto target = try_rays_with_state(_queen)) {
				return *target;
			}
			break;
		}

		case MoveGeneratorState::ITER_ROOK: {
			if (const auto target = try_rays_with_state(_rook)) {
				return *target;
			}
			break;
		}

		case MoveGeneratorState::ITER_BISHOP: {
			if (const auto target = try_rays_with_state(_bishop)) {
				return *target;
			}
			break;
		}

		case MoveGeneratorState::ITER_KNIGHT: {
			while (_knight.move_idx < _knight.valid_offsets.size()) {
				const auto [x, y] = _knight.valid_offsets[_knight.move_idx++];
				if (const auto target = try_move_or_take(_loc.offset(x, y))) {
					return *target;
				}
			}
			break;
		}

		case MoveGeneratorState::ITER_PAWN: {
			const bool is_starting_position = _loc.y == 1;
			while (_pawn.state != PawnMoveIterator::DONE) {
				switch (PawnMoveIterator::GeneratorState(_pawn.state++)) {
					// FIXME: holy hell en passant
				case PawnMoveIterator::TWO_AHEAD:
					if (is_starting_position && get_offset(0, 1)->is_empty() && get_offset(0, 2)->is_empty()) {
						return a_move_to(_loc.offset(0, 2));
					}
					break;
				case PawnMoveIterator::ONE_AHEAD:
					if (const auto target = try_empty_move(_loc.offset(0, 1))) {
						return *target;
					}
					break;
				case PawnMoveIterator::TAKE_LEFT:
					if (const auto target = try_take(_loc.offset(-1, 1))) {
						return *target;
					}
					break;
				case PawnMoveIterator::TAKE_RIGHT:
					if (const auto target = try_take(_loc.offset(1, 1))) {
						return *target;
					}
					break;
				case PawnMoveIterator::DONE: break;
				}
			}
			break;
		}

		default: assert(false && "unexpected state");
		}

		// at this point, exhausted possible moves for _current state
		skip_to_next_iterable();
	}

	return invalid_move_magic;
}

void MoveGenerator::find_first_iterable() {
	if (_current != MoveGeneratorState::READY) {
		return;
	}

	for (;;) {
		const Cell cell = *get_at(_loc);

		if (cell.is_ours) {
			switch (cell.cell.p) {
			case PieceKind::NONE: break;
			case PieceKind::KING:
				_current = MoveGeneratorState::ITER_KING;
				_king    = {};
				return;
			case PieceKind::QUEEN:
				_current = MoveGeneratorState::ITER_QUEEN;
				_queen   = {};
				return;
			case PieceKind::ROOK:
				_current = MoveGeneratorState::ITER_ROOK;
				_rook    = {};
				return;
			case PieceKind::BISHOP:
				_current = MoveGeneratorState::ITER_BISHOP;
				_bishop  = {};
				return;
			case PieceKind::KNIGHT:
				_current = MoveGeneratorState::ITER_KNIGHT;
				_knight  = {};
				return;
			case PieceKind::PAWN:
				_current = MoveGeneratorState::ITER_PAWN;
				_pawn    = {};
				return;
			}
		}

		if (!skip_to_next_cell()) {
			_current = MoveGeneratorState::FINISHED;
			return;
		}
	}
}

void MoveGenerator::skip_to_next_iterable() {
	if (!skip_to_next_cell()) {
		_current = MoveGeneratorState::FINISHED;
		return;
	}

	_current = MoveGeneratorState::READY;
	find_first_iterable();
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
#pragma once

#include <raclette/location.hpp>
#include <raclette/move.hpp>
#include <raclette/position.hpp>

#include <cstdint>
#include <optional>

namespace raclette {

class Position;

struct KingMoveState {};
struct QueenMoveState {};
struct RookMoveState {};
struct BishopMoveState {};
struct KnightMoveState {};
struct PawnMoveState {
	enum GeneratorState : std::uint8_t { TWO_AHEAD = 0, ONE_AHEAD, TAKE_LEFT, TAKE_RIGHT, DONE };

	std::uint8_t state = TWO_AHEAD;
};

class MoveGenerator {
	public:
	explicit MoveGenerator(const Position& position) : _position(&position) {}

	Move next();

	void seek(GlobalLocation loc) { _loc = loc.to_relative(true); }

	private:
	bool skip_to_next_cell();

	[[nodiscard]] std::optional<Cell> get_at(SideRelativeLocation loc) const {
		return _position->get(loc.to_global(true));
	}

	[[nodiscard]] std::optional<Cell> get_offset(int offset_x, int offset_y) const {
		return get_at(_loc.offset(offset_x, offset_y));
	}

	[[nodiscard]] std::optional<Move> try_simple_move(SideRelativeLocation to) {
		if (const auto candidate = get_at(to); candidate && candidate->is_empty()) {
			return {a_move_to(to)};
		}
		return std::nullopt;
	}

	[[nodiscard]] std::optional<Move> try_simple_take(SideRelativeLocation to) {
		if (const auto candidate = get_at(to); candidate && candidate->can_take()) {
			return {a_move_to(to)};
		}
		return std::nullopt;
	}

	[[nodiscard]] Move a_move_to(SideRelativeLocation to) const {
		return Move{_loc.to_global(true), to.to_global(true)};
	}

	const Position*      _position;
	SideRelativeLocation _loc{};

	KingMoveState   _king;
	QueenMoveState  _queen;
	RookMoveState   _rook;
	BishopMoveState _bishop;
	KnightMoveState _knight;
	PawnMoveState   _pawn;
};

} // namespace raclette
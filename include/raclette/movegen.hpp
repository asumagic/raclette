#pragma once

#include <raclette/location.hpp>
#include <raclette/move.hpp>
#include <raclette/position.hpp>

#include <array>
#include <cstdint>
#include <optional>
#include <utility>

namespace raclette {

class Position;

struct KingMoveIterator {
	static constexpr std::array<std::pair<int, int>, 8> valid_offsets{
	    {{-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}}
	};

	std::uint8_t move_idx = 0;
};

struct QueenMoveIterator {
	static constexpr std::array<std::pair<int, int>, 8> ray_directions{
	    {{-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}}
	};

	std::uint8_t ray_idx  = 0;
	std::uint8_t ray_dist = 0;
};

struct RookMoveIterator {
	static constexpr std::array<std::pair<int, int>, 4> ray_directions{{{-1, 0}, {0, -1}, {1, 0}, {0, 1}}};

	std::uint8_t ray_idx  = 0;
	std::uint8_t ray_dist = 0;
};

struct BishopMoveIterator {
	static constexpr std::array<std::pair<int, int>, 4> ray_directions{{{-1, 1}, {1, 1}, {1, -1}, {-1, -1}}};

	std::uint8_t ray_idx  = 0;
	std::uint8_t ray_dist = 0;
};

struct KnightMoveIterator {
	static constexpr std::array<std::pair<int, int>, 8> valid_offsets{
	    {{-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}}
	};

	std::uint8_t move_idx = 0;
};

struct PawnMoveIterator {
	enum GeneratorState : std::uint8_t { TWO_AHEAD = 0, ONE_AHEAD, TAKE_LEFT, TAKE_RIGHT, DONE };

	std::uint8_t state = TWO_AHEAD;
};

enum class MoveGeneratorState : std::uint8_t {
	READY,
	ITER_KING,
	ITER_QUEEN,
	ITER_ROOK,
	ITER_BISHOP,
	ITER_KNIGHT,
	ITER_PAWN,
	FINISHED
};

class MoveGenerator {
	public:
	explicit MoveGenerator(const Position& position) : _position(&position) {}

	Move next();

	void seek(GlobalLocation loc) { _loc = loc.to_relative(true); }

	private:
	void find_first_iterable();
	void skip_to_next_iterable();
	bool skip_to_next_cell();

	[[nodiscard]] std::optional<Cell> get_at(SideRelativeLocation loc) const {
		return _position->get(loc.to_global(true));
	}

	[[nodiscard]] std::optional<Cell> get_offset(int offset_x, int offset_y) const {
		return get_at(_loc.offset(offset_x, offset_y));
	}

	[[nodiscard]] std::optional<Move> try_empty_move(SideRelativeLocation to) {
		if (const auto candidate = get_at(to); candidate && candidate->is_empty()) {
			return {a_move_to(to)};
		}
		return std::nullopt;
	}

	[[nodiscard]] std::optional<Move> try_move_or_take(SideRelativeLocation to) {
		if (const auto candidate = get_at(to); candidate && (candidate->is_empty() || !candidate->is_ours)) {
			return {a_move_to(to)};
		}
		return std::nullopt;
	}

	[[nodiscard]] std::optional<Move> try_take(SideRelativeLocation to) {
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

	MoveGeneratorState _current = MoveGeneratorState::READY;

	// TODO: evaluate perf benefit of making these an union
	KingMoveIterator   _king;
	QueenMoveIterator  _queen;
	RookMoveIterator   _rook;
	BishopMoveIterator _bishop;
	KnightMoveIterator _knight;
	PawnMoveIterator   _pawn;
};

} // namespace raclette
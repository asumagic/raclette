#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <raclette/location.hpp>
#include <raclette/move.hpp>

namespace raclette {

enum class PieceKind : std::uint8_t { NONE, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

struct CellStorage {
	PieceKind p;
};

struct Cell {
	PieceKind            p;
	SideRelativeLocation loc;
	bool                 is_ours;

	[[nodiscard]] bool is_empty() const { return p == PieceKind::NONE; }
	[[nodiscard]] bool can_take() const { return !is_empty() && !is_ours; }
};

class Position {
	public:
	using Storage = std::array<CellStorage, 64>;

	Position()                           = default;
	~Position()                          = default;
	Position(const Position&)            = default;
	Position& operator=(const Position&) = default;
	Position(Position&&)                 = delete;
	Position& operator=(Position&&)      = delete;

	template<class Self> constexpr auto& at_side(this Self& self, SideRelativeLocation loc, bool in_ours) {
		auto y_loc    = in_ours ? loc.y : 7 - loc.y;
		bool in_white = in_ours ^ self.ours_is_white;
		return (in_white ? self.white : self.black)[(y_loc * 8) + loc.x];
	}

	template<class Self> constexpr Cell get_unsafe(this Self& self, SideRelativeLocation loc) {
		Cell cell{};
		cell.loc = loc;

		if (auto& our_cell = self.at_side(loc, true); our_cell.p != PieceKind::NONE) {
			cell.p       = our_cell.p;
			cell.is_ours = true;
		} else if (auto& their_cell = self.at_side(loc, false); their_cell.p != PieceKind::NONE) {
			cell.p       = their_cell.p;
			cell.is_ours = false;
		} else {
			cell.p = {PieceKind::NONE};
		}
		return cell;
	}

	template<class Self> constexpr std::optional<Cell> get(this Self& self, SideRelativeLocation loc) {
		if (!loc.is_in_bounds()) {
			return std::nullopt;
		}

		return {self.get_unsafe(loc)};
	}

	constexpr void set(SideRelativeLocation loc, PieceKind p, bool in_ours) { at_side(loc, in_ours).p = p; }

	void play_move(Move m) {
		// FIXME: en passant
		// FIXME: castling

		const auto from_piece = get_unsafe(m.from);
		// const auto to_piece = get_unsafe(m.to);

		set(m.from, PieceKind::NONE, false);
		set(m.to, from_piece.p, from_piece.is_ours);

		set_white_playing(!is_white_playing());
	}

	[[nodiscard]] GlobalLocation as_global_pos(SideRelativeLocation loc, bool is_ours) const {
		return {loc.x, !(ours_is_white == is_ours) ? loc.y : (7 - loc.y)};
	}

	[[nodiscard]] bool is_white_playing() const { return ours_is_white; }
	void               set_white_playing(bool value) { ours_is_white = value; }

	private:
	// both halves of storage represent a playing side from their own position i.e. (0, 0) starts as the rook of that
	// side
	Storage white = {}, black = {};
	bool    ours_is_white = true;
};

void set_initial_position(Position& position);

} // namespace raclette
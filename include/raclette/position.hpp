#pragma once

#include "raclette/location.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <optional>

namespace raclette {

enum class Piece : std::uint8_t { NONE, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

struct CellStorage {
	Piece p;
};

struct Cell {
	CellStorage    cell;
	GlobalLocation loc;
	bool           is_ours;

	[[nodiscard]] bool is_empty() const { return cell.p == Piece::NONE; }
	[[nodiscard]] bool can_take() const { return !is_empty() && !is_ours; }
};

class Position {
	public:
	using Storage = std::array<CellStorage, 64>;

	template<class Self> constexpr auto& at_relative_pos(this Self& self, SideRelativeLocation loc, bool in_ours) {
		return (in_ours ? self.ours : self.theirs)[(loc.y * 8) + loc.x];
	}

	template<class Self> constexpr auto& at_global_pos(this Self& self, GlobalLocation loc, bool in_ours) {
		return self.at_relative_pos({loc.x, in_ours ? loc.y : 7 - loc.y}, in_ours);
	}

	template<class Self> constexpr Cell get_unsafe(this Self& self, GlobalLocation loc) {
		Cell p{};
		p.loc = loc;
		if (auto& our_cell = self.at_global_pos(loc, true); our_cell.p != Piece::NONE) {
			p.cell    = our_cell;
			p.is_ours = true;
		} else if (auto& their_cell = self.at_global_pos(loc, false); their_cell.p != Piece::NONE) {
			p.cell    = their_cell;
			p.is_ours = false;
		} else {
			p.cell = {Piece::NONE};
		}
		return p;
	}

	template<class Self> constexpr std::optional<Cell> get(this Self& self, GlobalLocation loc) {
		if (!loc.is_in_bounds()) {
			return std::nullopt;
		}

		return {self.get_unsafe(loc)};
	}

	constexpr void set(GlobalLocation loc, Piece p, bool in_ours) { at_global_pos(loc, in_ours).p = p; }

	private:
	// both halves of storage represent a playing side from their own position i.e. (0, 0) starts as the rook of that
	// side
	Storage ours = {}, theirs = {};
};

void set_initial_position(Position& position);

} // namespace raclette
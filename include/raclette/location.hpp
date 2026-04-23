#pragma once

#include <fmt/format.h>

namespace raclette {

struct SideRelativeLocation;

/// Location of a cell on the board in terms of global coordinates (not relative to a side), where (0..7, 0..7)
/// represent (a..h, 1..8) positions
struct GlobalLocation {
	int x, y;

	bool operator==(const GlobalLocation&) const = default;

	[[nodiscard]] bool is_in_bounds() const { return x >= 0 && x <= 7 && y >= 0 && y <= 7; }

	[[nodiscard]] SideRelativeLocation to_relative(bool is_ours) const;
};

struct SideRelativeLocation {
	int x, y;

	bool operator==(const SideRelativeLocation&) const = default;

	[[nodiscard]] SideRelativeLocation offset(int offset_x, int offset_y) const { return {x + offset_x, y + offset_y}; }

	[[nodiscard]] bool is_in_bounds() const { return x >= 0 && x <= 7 && y >= 0 && y <= 7; }

	[[nodiscard]] GlobalLocation to_global(bool is_ours) const;
};

inline SideRelativeLocation GlobalLocation::to_relative(bool is_ours) const { return {x, is_ours ? y : 7 - y}; }
inline GlobalLocation       SideRelativeLocation::to_global(bool is_ours) const { return {x, is_ours ? y : 7 - y}; }

} // namespace raclette

template<> class fmt::formatter<raclette::GlobalLocation> {
	public:
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template<typename Context> constexpr auto format(raclette::GlobalLocation const& loc, Context& ctx) const {
		return format_to(ctx.out(), "{}{}", char('a' + loc.x), loc.y + 1);
	}
};
#pragma once

#include <raclette/location.hpp>
namespace raclette {

struct Move {
	GlobalLocation from, to;

	[[nodiscard]] bool is_valid() const { return from.x != -1; }

	bool operator==(const Move&) const = default;
};

static constexpr Move invalid_move_magic = {{-1, -1}, {-1, -1}};

} // namespace raclette
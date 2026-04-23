#pragma once

#include <raclette/position.hpp>

#include <string_view>

namespace raclette::parser {

void parse_fen_position(Position& position, std::string_view fen);

}
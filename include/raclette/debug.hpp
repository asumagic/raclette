#pragma once

#include <raclette/move.hpp>

#include <vector>

namespace raclette {

struct Cell;
class Position;

void print_piece_pretty(Cell cell);
void print_board_pretty(const Position& position, const std::vector<Move>& highlighted_moves = {});

} // namespace raclette
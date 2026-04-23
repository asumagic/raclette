#pragma once

#include <raclette/move.hpp>

#include <vector>

namespace raclette {

struct Cell;
class Position;

void print_piece_pretty(Cell p);
void print_board_pretty(const Position& board, const std::vector<Move>& highlighted_moves = {});

} // namespace raclette
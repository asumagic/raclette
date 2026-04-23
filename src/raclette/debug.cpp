#include <raclette/board.hpp>
#include <raclette/debug.hpp>

#include <fmt/core.h>

namespace raclette {

void print_piece_pretty(Position p) {
    if ((p.x + p.y) % 2 != 0) {
        fmt::print("\e[0;100m");
    } else {
        fmt::print("\e[0;107m");
    }
    if (!p.is_ours) {
        switch (p.cell.p) {
        case Piece::NONE:   fmt::print(" "); break;
        case Piece::KING:   fmt::print("♚"); break;
        case Piece::QUEEN:  fmt::print("♛"); break;
        case Piece::ROOK:   fmt::print("♜"); break;
        case Piece::BISHOP: fmt::print("♝"); break;
        case Piece::KNIGHT: fmt::print("♞"); break;
        case Piece::PAWN:   fmt::print("♟"); break;
        }
    } else {
        switch (p.cell.p) {
        case Piece::NONE:   fmt::print(" "); break;
        case Piece::KING:   fmt::print("♔"); break;
        case Piece::QUEEN:  fmt::print("♕"); break;
        case Piece::ROOK:   fmt::print("♖"); break;
        case Piece::BISHOP: fmt::print("♗"); break;
        case Piece::KNIGHT: fmt::print("♘"); break;
        case Piece::PAWN:   fmt::print("♙"); break;
        }
    }
    fmt::print(" \e[0m");
}

void print_board_pretty(const Board& board) {
    fmt::println("  a b c d e f g h");
    for (int y = 0; y < 8; ++y) {
        fmt::print("{} ", 7 - y + 1);
        for (int x = 0; x < 8; ++x) {
            print_piece_pretty(board.get_at(x, y));
        }
        fmt::println("{}", 7 - y + 1);
    }
    fmt::println("  a b c d e f g h");
}

}
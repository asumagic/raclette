#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <tuple>

namespace raclette {

enum class Piece : std::uint8_t {
    NONE, KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN
};

struct Cell {
    Piece p;

    explicit operator bool() const {
        return p != Piece::NONE;
    }
};

struct Position {
    Cell cell;
    std::uint8_t x, y;
    bool is_ours;
};

class Board {
    public:
    using Storage = std::array<Cell, 8*8>;

    template<class Self>
    constexpr auto& at_in_side(this Self&& self, int x, int y, bool in_ours) {
        assert(x >= 0);
        assert(x <= 7);
        assert(y >= 0);
        assert(y <= 7);

        if (in_ours) {
            return self.ours[y * 8 + x];
        } else {
            return self.theirs[(7 - y) * 8 + x];
        }
    }

    template<class Self>
    constexpr Position get_at(this Self&& self, int x, int y) {
        Position p;
        p.x = std::uint8_t(x);
        p.y = std::uint8_t(y);
        p.is_ours = false;
        if (auto& our_cell = self.at_in_side(x, y, true); bool(our_cell)) {
            p.cell = our_cell;
            p.is_ours = true;
        } else if (auto& their_cell = self.at_in_side(x, y, false); bool(their_cell)) {
            p.cell = their_cell;
            p.is_ours = false;
        } else {
            p.cell = {Piece::NONE};
        }
        return p;
    }

    constexpr Board() {
        ours = theirs = {};

        static constexpr std::array<std::tuple<Piece, int, int>, 16> initial_positions {{
            {Piece::ROOK, 0, 0},
            {Piece::KNIGHT, 1, 0},
            {Piece::BISHOP, 2, 0},
            {Piece::QUEEN, 3, 0},
            {Piece::KING, 4, 0},
            {Piece::BISHOP, 5, 0},
            {Piece::KNIGHT, 6, 0},
            {Piece::ROOK, 7, 0},
            {Piece::PAWN, 0, 1},
            {Piece::PAWN, 1, 1},
            {Piece::PAWN, 2, 1},
            {Piece::PAWN, 3, 1},
            {Piece::PAWN, 4, 1},
            {Piece::PAWN, 5, 1},
            {Piece::PAWN, 6, 1},
            {Piece::PAWN, 7, 1},
        }};

        for (const auto [piece, x, y] : initial_positions) {
            at_in_side(x, y, true).p = piece;
            at_in_side(x, 7 - y, false).p = piece;
        }
    }

    private:
    // both boards are represented from their own position
    Storage ours, theirs;
};

}
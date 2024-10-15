#ifndef MOVE_H
#define MOVE_H

#include <iostream>

#include "Constants.hpp"

struct Move {
    uint64_t start;
    uint64_t end;
    Move() = default;

    Move(uint64_t start, uint64_t end)
        : start(start)
        , end(end) {}


    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << "(" << __builtin_ctzll(move.start) / boardSize << ", " << __builtin_ctzll(move.start) % boardSize << ")"
           << " -> "
           << "(" << __builtin_ctzll(move.end) / boardSize << ", " << __builtin_ctzll(move.end) % boardSize << ")"
           << "\n";


        return os;
    };
};

#endif

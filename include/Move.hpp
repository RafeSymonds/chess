#ifndef MOVE_H
#define MOVE_H

#include <iostream>

#include "Constants.hpp"

struct Move {
    int start;
    int end;
    Move() = default;

    Move(int start, int end)
        : start(start)
        , end(end) {}


    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << "(" << move.start / boardSize << ", " << move.start % boardSize << ")"
           << " -> "
           << "(" << move.end / boardSize << ", " << move.end % boardSize << ")"
           << "\n";


        return os;
    };
};

#endif

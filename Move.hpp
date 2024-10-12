#ifndef MOVE_H
#define MOVE_H

#include <iostream>

#include "Position.hpp"

struct Move {
    Position start;
    Position end;
    Move() = default;

    Move(const Position& start, const Position& end)
        : start(start)
        , end(end) {}

    Move(int rowStart, int columnStart, int rowEnd, int columnEnd)
        : start(rowStart, columnStart)
        , end(rowEnd, columnEnd) {}

    friend std::ostream& operator<<(std::ostream& os, const Move& move) {
        os << "Start r=" << move.start.row << " c=" << move.start.column;
        os << "End r=" << move.end.row << " c=" << move.end.column;
        return os;
    };
};

#endif

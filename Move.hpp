#ifndef MOVE_H
#define MOVE_H

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
};


#endif

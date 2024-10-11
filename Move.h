#ifndef MOVE_H
#define MOVE_H

#include "Position.h"

struct Move {
    Position start;
    Position end;
    Move() = default;

    Move(Position& start, Position& end)
        : start(start)
        , end(end) {}

    Move(short rowStart, short columnStart, short rowEnd, short columnEnd)
        : start(rowStart, columnStart)
        , end(rowEnd, columnEnd) {}
};


#endif

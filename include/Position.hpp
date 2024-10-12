#ifndef POSITION_H
#define POSITION_H

#include <vector>

#include "Constants.hpp"

struct Position {
    int row;
    int column;

    Position() = default;

    Position(int r, int c)
        : row(r)
        , column(c) {}

    bool operator==(const Position& other) const { return row == other.row && column == other.column; }
    Position operator+(const Position& other) const { return { row + other.row, column + other.column }; }
};

inline bool validPosition(const Position& position) {
    return position.row < boardSize && position.row >= 0 && position.column < boardSize && position.column >= 0;
}
inline bool validPosition(int row, int column) {
    return row < boardSize && row >= 0 && column < boardSize && column >= 0;
}


const std::vector<Position> straightDirections = {
    {  1,  0 }, // Down
    { -1,  0 }, // Up
    {  0,  1 }, // Right
    {  0, -1 }  // Left
};

// Define directions for diagonal movement (Bishops and Queens)
const std::vector<Position> diagonalDirections = {
    {  1,  1 }, // Bottom-Right
    {  1, -1 }, // Bottom-Left
    { -1,  1 }, // Top-Right
    { -1, -1 }  // Top-Left
};

const static std::vector<Position> knightMoves = {
    { { 2, 1 }, { 2, -1 }, { -2, 1 }, { -2, -1 }, { 1, 2 }, { 1, -2 }, { -1, 2 }, { -1, -2 } }
};


#endif

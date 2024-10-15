#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

constexpr int boardSize = 8;
constexpr int numBoardSquares = 64;
const std::string defaultBoardPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
constexpr int numTypesPieces = 12;


enum PieceTypes : std::int8_t {
    blackPawn,
    blackKnight,
    blackBishop,
    blackRook,
    blackQueen,
    blackKing,
    whitePawn,
    whiteKnight,
    whiteBishop,
    whiteRook,
    whiteQueen,
    whiteKing
};

constexpr uint64_t pawnAttackingLeft = ~0x8080808080808080;
constexpr uint64_t pawnAttackingRight = ~0x0101010101010101;


constexpr uint64_t rowMasks(int row) {
    return 0xFFULL << (row * 8);
}
constexpr uint64_t columnMasks(int column) {
    return 0x0101010101010101ULL << column;
}


struct Position {
    int row;
    int column;
    Position(int row, int column)
        : row(row)
        , column(column) {}
};

constexpr std::array<int, 8> knightOffsets = { 17, 15, 10, 6, -6, -10, -15, -17 };
constexpr std::array<int, 4> straightDirections = { -boardSize, boardSize, -1, 1 };
constexpr std::array<int, 4> diagonalDirections = { -boardSize - 1, -boardSize + 1, boardSize - 1, boardSize + 1 };

const std::vector<Position> kingMoves = {
    { -1, -1 }, // Top-Left
    { -1,  0 }, // Up
    { -1,  1 }, // Top-Right
    {  0, -1 }, // Left
    {  0,  1 }, // Right
    {  1, -1 }, // Bottom-Left
    {  1,  0 }, // Down
    {  1,  1 }  // Bottom-Righ}
};


inline char getCharFromPieceType(PieceTypes piece) {
    switch (piece) {
    case blackPawn:
        return 'p';
    case whitePawn:
        return 'P';
    case blackKnight:
        return 'n';
    case whiteKnight:
        return 'N';
    case blackBishop:
        return 'b';
    case whiteBishop:
        return 'B';
    case blackRook:
        return 'r';
    case whiteRook:
        return 'R';
    case blackQueen:
        return 'q';
    case whiteQueen:
        return 'Q';
    case blackKing:
        return 'k';
    case whiteKing:
        return 'K';
    default:
        return '.';
    }
}
const std::vector<std::vector<int>> whitePawnPositionValues = {
    {  0,  0,   0,   0,   0,   0,  0,  0 },
    { 50, 50,  50,  50,  50,  50, 50, 50 },
    { 10, 10,  20,  30,  30,  20, 10, 10 },
    {  5,  5,  10,  25,  25,  10,  5,  5 },
    {  0,  0,   0,  20,  20,   0,  0,  0 },
    {  5, -5, -10,   0,   0, -10, -5,  5 },
    {  5, 10,  10, -20, -20,  10, 10,  5 },
    {  0,  0,   0,   0,   0,   0,  0,  0 }
};

const std::vector<std::vector<int>> blackPawnPositionValues = {
    {   0,   0,   0,   0,   0,   0,   0,   0 },
    {  -5, -10, -10,  20,  20, -10, -10,  -5 },
    {  -5,   5,  10,   0,   0,  10,   5,  -5 },
    {   0,   0,   0, -20, -20,   0,   0,   0 },
    {  -5,  -5, -10, -25, -25, -10,  -5,  -5 },
    { -10, -10, -20, -30, -30, -20, -10, -10 },
    { -50, -50, -50, -50, -50, -50, -50, -50 },
    {   0,   0,   0,   0,   0,   0,   0,   0 }
};


const std::vector<std::vector<int>> whiteKnightPositionValues = {
    { -50, -40, -30, -30, -30, -30, -40, -50 },
    { -40, -20,   0,   0,   0,   0, -20, -40 },
    { -30,   0,  10,  15,  15,  10,   0, -30 },
    { -30,   5,  15,  20,  20,  15,   5, -30 },
    { -30,   0,  15,  20,  20,  15,   0, -30 },
    { -30,   5,  10,  15,  15,  10,   5, -30 },
    { -40, -20,   0,   5,   5,   0, -20, -40 },
    { -50, -40, -30, -30, -30, -30, -40, -50 }
};
const std::vector<std::vector<int>> blackKnightPositionValues = {
    { 50, 40,  30,  30,  30,  30, 40, 50 },
    { 40, 20,   0,  -5,  -5,   0, 20, 40 },
    { 30, -5, -10, -15, -15, -10, -5, 30 },
    { 30,  0, -15, -20, -20, -15,  0, 30 },
    { 30, -5, -15, -20, -20, -15, -5, 30 },
    { 30,  0, -10, -15, -15, -10,  0, 30 },
    { 40, 20,   0,   0,   0,   0, 20, 40 },
    { 50, 40,  30,  30,  30,  30, 40, 50 }
};

const std::vector<std::vector<int>> whiteBishopPositionValues = {
    { -20, -10, -10, -10, -10, -10, -10, -20 },
    { -10,   0,   0,   0,   0,   0,   0, -10 },
    { -10,   0,   5,  10,  10,   5,   0, -10 },
    { -10,   5,   5,  10,  10,   5,   5, -10 },
    { -10,   0,  10,  10,  10,  10,   0, -10 },
    { -10,  10,  10,  10,  10,  10,  10, -10 },
    { -10,   5,   0,   0,   0,   0,   5, -10 },
    { -20, -10, -10, -10, -10, -10, -10, -20 }
};

const std::vector<std::vector<int>> blackBishopPositionValues = {
    { 20,  10,  10,  10,  10,  10,  10, 20 },
    { 10,  -5,   0,   0,   0,   0,  -5, 10 },
    { 10,  -5,  -5, -10, -10,  -5,  -5, 10 },
    { 10, -10, -10, -10, -10, -10, -10, 10 },
    { 10,   0, -10, -10, -10, -10,   0, 10 },
    { 10,   0,  -5, -10, -10,  -5,   0, 10 },
    { 10,   0,   0,   0,   0,   0,   0, 10 },
    { 20,  10,  10,  10,  10,  10,  10, 20 }
};

const std::vector<std::vector<int>> whiteRookPositionValues = {
    {  0,  0,  0,  0,  0,  0,  0,  0 },
    {  5, 10, 10, 10, 10, 10, 10,  5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    { -5,  0,  0,  0,  0,  0,  0, -5 },
    {  0,  0,  0,  5,  5,  0,  0,  0 }
};

const std::vector<std::vector<int>> blackRookPositionValues = {
    {  0,   0,   0,  -5,  -5,   0,   0,  0 },
    {  5,   0,   0,   0,   0,   0,   0,  5 },
    {  5,   0,   0,   0,   0,   0,   0,  5 },
    {  5,   0,   0,   0,   0,   0,   0,  5 },
    {  5,   0,   0,   0,   0,   0,   0,  5 },
    {  5,   0,   0,   0,   0,   0,   0,  5 },
    { -5, -10, -10, -10, -10, -10, -10, -5 },
    {  0,   0,   0,   0,   0,   0,   0,  0 }
};

const std::vector<std::vector<int>> whiteQueenPositionValues = {
    { -20, -10, -10, -5, -5, -10, -10, -20 },
    { -10,   0,   0,  0,  0,   0,   0, -10 },
    { -10,   0,   5,  5,  5,   5,   0, -10 },
    {  -5,   0,   5,  5,  5,   5,   0,  -5 },
    {   0,   0,   5,  5,  5,   5,   0,  -5 },
    { -10,   5,   5,  5,  5,   5,   0, -10 },
    { -10,   0,   5,  0,  0,   0,   0, -10 },
    { -20, -10, -10, -5, -5, -10, -10, -20 }
};

const std::vector<std::vector<int>> blackQueenPositionValues = {
    { 20, 10, 10,  5,  5, 10, 10, 20 },
    { 10,  0, -5,  0,  0,  0,  0, 10 },
    { 10, -5, -5, -5, -5, -5,  0, 10 },
    {  0,  0, -5, -5, -5, -5,  0,  5 },
    {  5,  0, -5, -5, -5, -5,  0,  5 },
    { 10,  0, -5, -5, -5, -5,  0, 10 },
    { 10,  0,  0,  0,  0,  0,  0, 10 },
    { 20, 10, 10,  5,  5, 10, 10, 20 }
};

const std::vector<std::vector<int>> whiteKingMiddleGamePositionValues = {
    { -30, -40, -40, -50, -50, -40, -40, -30 },
    { -30, -40, -40, -50, -50, -40, -40, -30 },
    { -30, -40, -40, -50, -50, -40, -40, -30 },
    { -30, -40, -40, -50, -50, -40, -40, -30 },
    { -20, -30, -30, -40, -40, -30, -30, -20 },
    { -10, -20, -20, -20, -20, -20, -20, -10 },
    {  20,  20,   0,   0,   0,   0,  20,  20 },
    {  20,  30,  10,   0,   0,  10,  30,  20 }
};

const std::vector<std::vector<int>> blackKingMiddleGamePositionValues = {
    { -20, -30, -10,  0,  0, -10, -30, -20 },
    { -20, -20,   0,  0,  0,   0, -20, -20 },
    {  10,  20,  20, 20, 20,  20,  20,  10 },
    {  20,  30,  30, 40, 40,  30,  30,  20 },
    {  30,  40,  40, 50, 50,  40,  40,  30 },
    {  30,  40,  40, 50, 50,  40,  40,  30 },
    {  30,  40,  40, 50, 50,  40,  40,  30 },
    {  30,  40,  40, 50, 50,  40,  40,  30 }
};

inline double getValueFromPieceType(PieceTypes piece, int row, int column) {
    switch (piece) {
    case whitePawn:
        return 100 + whitePawnPositionValues[row][column];
    case blackPawn:
        return -100 + blackPawnPositionValues[row][column];
    case whiteKnight:
        return 300 + whiteKnightPositionValues[row][column];
    case blackKnight:
        return -300 + blackKnightPositionValues[row][column];
    case whiteBishop:
        return 300 + whiteBishopPositionValues[row][column];
    case blackBishop:
        return -300 + blackBishopPositionValues[row][column];
    case whiteRook:
        return 500 + whiteRookPositionValues[row][column];
    case blackRook:
        return -500 + blackRookPositionValues[row][column];
    case whiteQueen:
        return 900 + whiteQueenPositionValues[row][column];
    case blackQueen:
        return -900 + blackQueenPositionValues[row][column];
    case whiteKing:
        return 20000 + whiteKingMiddleGamePositionValues[row][column];
    case blackKing:
        return -20000 + blackKingMiddleGamePositionValues[row][column];
    default:
        return 0;
    }
}


#endif

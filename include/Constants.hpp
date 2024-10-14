#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>
#include <cstdint>
#include <string>
#include <vector>


constexpr int boardSize = 8;
constexpr int numBoardSquares = 64;
const std::string defaultBoardPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";


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
    return 0xFFULL << row;
}
constexpr uint64_t columnMasks(int column) {
    return 0x0101010101010101ULL << column;
}


constexpr std::array<uint64_t, numBoardSquares> knightMoves() {
    std::array<uint64_t, numBoardSquares> moves {};

    for (int i = 0; i < numBoardSquares; ++i) {
        uint64_t move = 0;

        // Check if moving left 2 is possible
        if (i % boardSize >= 2) {
            if (i / boardSize >= 1) {
                move |= (1ULL << (i - (2 * boardSize) - 1));   // left 2 up 1
            }
            if (i / boardSize <= boardSize - 2) {
                move |= (1ULL << (i + (2 * boardSize) - 1));   // left 2 down 1
            }
        }

        // Check if moving left 1 is possible
        if (i % boardSize >= 1) {
            if (i / boardSize >= 2) {
                move |= (1ULL << (i - (boardSize + 1)));   // left 1 up 2
            }
            if (i / boardSize <= boardSize - 3) {
                move |= (1ULL << (i + (boardSize - 1)));   // left 1 down 2
            }
        }

        // Check if moving right 1 is possible
        if (i % boardSize <= boardSize - 2) {
            if (i / boardSize >= 2) {
                move |= (1ULL << (i - (boardSize - 1)));   // right 1 up 2
            }
            if (i / boardSize <= boardSize - 3) {
                move |= (1ULL << (i + (boardSize + 1)));   // right 1 down 2
            }
        }

        // Check if moving right 2 is possible
        if (i % boardSize <= boardSize - 3) {
            if (i / boardSize >= 1) {
                move |= (1ULL << (i - (2 * boardSize) + 1));   // right 2 up 1
            }
            if (i / boardSize <= boardSize - 2) {
                move |= (1ULL << (i + (2 * boardSize) - 1));   // right 2 down 1
            }
        }

        moves.at(i) = move;
    }

    return moves;
};

struct Position {
    int row;
    int column;
    Position(int row, int column)
        : row(row)
        , column(column) {}
};

const std::vector<int> straightDirections = { -boardSize, boardSize, -1, 1 };

// Define directions for diagonal movement (Bishops and Queens)
const std::vector<Position> diagonalDirections = {
    {  1,  1 }, // Bottom-Right
    {  1, -1 }, // Bottom-Left
    { -1,  1 }, // Top-Right
    { -1, -1 }  // Top-Left
};

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

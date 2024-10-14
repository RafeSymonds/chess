#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <string>
#include <vector>

enum PieceTypes : std::int8_t {
    blackPawn = -6,
    blackKnight,
    blackBishop,
    blackRook,
    blackQueen,
    blackKing,
    none,
    whitePawn,
    whiteKnight,
    whiteBishop,
    whiteRook,
    whiteQueen,
    whiteKing
};

inline PieceTypes getPieceTypeFromChar(char c) {
    switch (c) {
    case 'p':
        return blackPawn;
    case 'P':
        return whitePawn;
    case 'n':
        return blackKnight;
    case 'N':
        return whiteKnight;
    case 'b':
        return blackBishop;
    case 'B':
        return whiteBishop;
    case 'r':
        return blackRook;
    case 'R':
        return whiteRook;
    case 'q':
        return blackQueen;
    case 'Q':
        return whiteQueen;
    case 'k':
        return blackKing;
    case 'K':
        return whiteKing;
    default:
        return none;
    }
}
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

constexpr int boardSize = 8;
const std::string defaultBoardPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

inline bool sameSide(PieceTypes piece1, PieceTypes piece2) {
    return (piece1 > none && piece2 > none) || (piece1 < none && piece2 < none);
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
    case none:
    default:
        return 0;
    }
}


#endif

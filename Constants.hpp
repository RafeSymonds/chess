#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <string>

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

constexpr int boardSize = 8;
const std::string defaultBoardPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

inline bool sameSide(PieceTypes piece1, PieceTypes piece2) {
    return (piece1 > none && piece2 > none) || (piece1 < none && piece2 < none);
}

#endif

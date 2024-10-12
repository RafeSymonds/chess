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

#endif

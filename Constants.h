#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BOARD_SIZE 8
#define BOARD_DEFAULT_POSITION "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"


enum Pieces {
    blackPawn = -5,
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


#endif

#include "Board.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "Constants.hpp"

using namespace std;

Board::Board(const string& fen) {
    uint64_t pos = 1;

    size_t endOfBoardIndex = fen.find(' ');

    if (endOfBoardIndex == string::npos) {
        endOfBoardIndex = fen.size();
    }

    for (size_t i = 0; i < endOfBoardIndex; ++i) {
        if (isdigit(fen[i]) != 0) {
            pos <<= fen[i] - '0';
            continue;
        }

        switch (fen[i]) {
        case 'p':
            pieceBB[blackPawn] |= pos;
            break;

        case 'P':
            pieceBB[whitePawn] |= pos;
            break;

        case 'n':
            pieceBB[blackKnight] |= pos;
            break;

        case 'N':
            pieceBB[whiteKnight] |= pos;
            break;

        case 'b':
            pieceBB[blackBishop] |= pos;
            break;

        case 'B':
            pieceBB[whiteBishop] |= pos;
            break;

        case 'r':
            pieceBB[blackRook] |= pos;
            break;

        case 'R':
            pieceBB[whiteRook] |= pos;
            break;

        case 'q':
            pieceBB[blackQueen] |= pos;
            break;

        case 'Q':
            pieceBB[whiteQueen] |= pos;
            break;

        case 'k':
            pieceBB[blackKing] |= pos;
            break;

        case 'K':
            pieceBB[whiteKing] |= pos;
            break;

        default:
            continue;
        }
        pos <<= 1;
    }

    whitePieces = pieceBB[whitePawn] | pieceBB[whiteKnight] | pieceBB[whiteBishop] | pieceBB[whiteRook]
                | pieceBB[whiteQueen] | pieceBB[whiteKing];

    blackPieces = pieceBB[blackPawn] | pieceBB[blackKnight] | pieceBB[blackBishop] | pieceBB[blackRook]
                | pieceBB[blackQueen] | pieceBB[blackKing];
}


uint64_t Board::getWhitePawnAttacks() {
    return (pieceBB[whitePawn] >> 7 & pawnAttackingRight) | (pieceBB[whitePawn] >> 9 & pawnAttackingLeft);
}

uint64_t Board::getBlackPawnAttacks() {
    return (pieceBB[blackPawn] << 7 & pawnAttackingLeft) | (pieceBB[blackPawn] << 9 & pawnAttackingRight);
}

uint64_t Board::getKnightAttacks(bool white) {
    uint64_t knights = white ? pieceBB[whiteKnight] : pieceBB[blackKnight];

    uint64_t attacks = 0;

    int index = 0;

    while (knights > 0) {
        if ((knights & 1) == 1) {
            attacks |= knightMoves()[index];
        }
        ++index;
        knights >>= 1;
    }

    return attacks;
}

uint64_t Board::getRookAttacks(bool white) {
    uint64_t rooks = white ? pieceBB[whiteRook] : pieceBB[blackRook];

    uint64_t sameColor = white ? whitePieces : blackPieces;
    uint64_t oppositeColor = white ? blackPieces : whitePieces;

    uint64_t attacks = 0;


    uint64_t currentPos = 1;

    while (rooks > 0) {
        if ((rooks & 1) == 0) {
            rooks >>= 1;
            currentPos <<= 1;
            continue;
        }
        for (int dir : straightDirections) {
            uint64_t newPos = currentPos;
            while (true) {
                newPos = newPos + dir;

                if (currentPos / boardSize < 0 || currentPos / boardSize >= boardSize
                    || (dir == -1 && (currentPos % boardSize == boardSize - 1))
                    || (dir == 1 && (currentPos % boardSize == 0))) {
                    break;
                }

                if ((sameColor & currentPos) == 1) {
                    break;
                }
                if ((oppositeColor & currentPos) == 1) {
                    attacks |= currentPos;
                    break;
                }
                attacks |= currentPos;
            }
        }
        rooks >>= 1;
        currentPos <<= 1;
    }
    return attacks;
}

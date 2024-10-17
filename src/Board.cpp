#include "Board.hpp"

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "Constants.hpp"
#include "Move.hpp"

using namespace std;

Board::Board(const string& fen, std::array<uint64_t, numBoardSquares>* knightMoves)
    : allPossibleMoves(100)
    , knightMoves(knightMoves) {
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

    for (int pieceType = blackPawn; pieceType <= whiteKing; ++pieceType) {
        uint64_t pieces = pieceBB[pieceType];

        while (pieces != 0) {
            int pos = __builtin_ctzll(pieces);
            currentEval += getValueFromPieceType(pieceType, pos);

            pieces &= pieces - 1;
        }
    }
};

uint64_t Board::getPawnAttacks(bool white) const {
    if (white) {
        return (pieceBB[whitePawn] >> (boardSize - 1) & pawnAttackingRight)
             | (pieceBB[whitePawn] >> (boardSize + 1) & pawnAttackingLeft);
    }

    return (pieceBB[blackPawn] << (boardSize - 1) & pawnAttackingLeft)
         | (pieceBB[blackPawn] << (boardSize + 1) & pawnAttackingRight);
}

void Board::getPawnMoves(bool white) {
    uint64_t emptySquares = ~(whitePieces | blackPieces);

    uint64_t pawns = white ? pieceBB[whitePawn] : pieceBB[blackPawn];

    if (pawns == 0) {
        return;
    }

    if (white) {
        uint64_t startRow = rowMasks(boardSize - 2);

        uint64_t singleStep = (pawns >> boardSize) & emptySquares;
        uint64_t doubleStep = ((pawns & startRow) >> boardSize * 2) & emptySquares & (singleStep >> boardSize);
        uint64_t attackLeft = pawns >> (boardSize + 1) & pawnAttackingLeft & blackPieces;
        uint64_t attackRight = pawns >> (boardSize - 1) & pawnAttackingRight & blackPieces;

        while (pawns != 0) {
            int startSquare = __builtin_ctzll(pawns);
            pawns &= pawns - 1;   // Clear the LS1B

            uint64_t startSquareMask = 1ULL << startSquare;

            // Single step forward move
            uint64_t target = startSquareMask >> boardSize;

            if ((singleStep & target) != 0) {
                allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
                allPossibleMoves[allPossibleMovesSize].end = target;
                ++allPossibleMovesSize;
            }

            // Double step forward move
            target = startSquareMask >> boardSize * 2;
            if ((doubleStep & target) != 0) {
                allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
                allPossibleMoves[allPossibleMovesSize].end = target;
                ++allPossibleMovesSize;
            }

            // Attack left
            target = startSquareMask >> (boardSize + 1);
            if ((attackLeft & target) != 0) {
                allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
                allPossibleMoves[allPossibleMovesSize].end = target;
                ++allPossibleMovesSize;
            }

            // Attack right
            target = startSquareMask >> (boardSize - 1);
            if ((attackRight & target) != 0) {
                allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
                allPossibleMoves[allPossibleMovesSize].end = target;
                ++allPossibleMovesSize;
            }
        }
    } else {
        uint64_t startRow = rowMasks(1);

        uint64_t singleStep = (pawns << boardSize) & emptySquares;
        uint64_t doubleStep = ((pawns & startRow) << (boardSize * 2)) & emptySquares & (singleStep << boardSize);
        uint64_t attackLeft = (pawns << (boardSize - 1)) & pawnAttackingRight & whitePieces;
        uint64_t attackRight = (pawns << (boardSize + 1)) & pawnAttackingLeft & whitePieces;

        while (pawns != 0) {
            int startSquare = __builtin_ctzll(pawns);
            pawns &= pawns - 1;

            uint64_t startSquareMask = 1ULL << startSquare;

            // Single step forward move
            uint64_t target = startSquareMask << boardSize;
            if ((singleStep & target) != 0) {
                allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
                allPossibleMoves[allPossibleMovesSize].end = target;
                ++allPossibleMovesSize;
            }

            // Double step forward move
            target = startSquareMask << (boardSize * 2);
            if ((doubleStep & target) != 0) {
                allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
                allPossibleMoves[allPossibleMovesSize].end = target;
                ++allPossibleMovesSize;
            }

            // Attack left
            target = startSquareMask << (boardSize - 1);
            if ((attackLeft & target) != 0) {
                allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
                allPossibleMoves[allPossibleMovesSize].end = target;
                ++allPossibleMovesSize;
            }

            // Attack right
            target = startSquareMask << (boardSize + 1);
            if ((attackRight & target) != 0) {
                allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
                allPossibleMoves[allPossibleMovesSize].end = target;
                ++allPossibleMovesSize;
            }
        }
    }
}

uint64_t Board::getKnightAttacks(bool white) const {
    uint64_t knights = white ? pieceBB[whiteKnight] : pieceBB[blackKnight];
    uint64_t attacks = 0;

    if (knights == 0) {
        return attacks;
    }

    while (knights != 0) {
        int startSquare = __builtin_ctzll(knights);
        knights &= knights - 1;

        attacks |= (*knightMoves)[startSquare];
    }

    return attacks;
}

void Board::getKnightMoves(bool white) {
    uint64_t knights = white ? pieceBB[whiteKnight] : pieceBB[blackKnight];

    if (knights == 0) {
        return;
    }

    uint64_t possibleSpots = ~(white ? whitePieces : blackPieces);


    while (knights != 0) {
        int startSquare = __builtin_ctzll(knights);
        knights &= knights - 1;

        uint64_t startSquareMask = 1ULL << startSquare;

        int row = startSquare / boardSize;
        int col = startSquare % boardSize;

        for (int offset : knightOffsets) {
            int targetSquare = startSquare + offset;

            if (targetSquare < 0 || targetSquare >= numBoardSquares) {
                continue;
            }

            int newRow = targetSquare / boardSize;
            int newCol = targetSquare % boardSize;

            int rowDiff = abs(newRow - row);
            int colDiff = abs(newCol - col);

            if ((rowDiff != 2 || colDiff != 1) && (rowDiff != 1 || colDiff != 2)) {
                continue;
            }

            uint64_t targetMask = (1ULL << targetSquare);

            if ((targetMask & possibleSpots) == 0) {
                continue;
            }

            allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
            allPossibleMoves[allPossibleMovesSize].end = targetMask;
            ++allPossibleMovesSize;
        }
    }
}


uint64_t Board::getStraightAttacks(uint64_t pieces, bool white) const {
    uint64_t attacks = 0;

    if (pieces == 0) {
        return attacks;
    }
    uint64_t sameColor = white ? whitePieces : blackPieces;
    uint64_t oppositeColor = white ? blackPieces : whitePieces;


    while (pieces > 0) {
        int startingPosition = __builtin_ctzll(pieces);
        pieces &= pieces - 1;

        for (int dir : straightDirections) {
            int newPos = startingPosition;
            while (true) {
                newPos = newPos + dir;

                if (newPos < 0 || newPos >= numBoardSquares) {
                    break;
                }

                if (dir == -1 && (newPos % boardSize == boardSize - 1)) {
                    break;
                }
                if (dir == 1 && (newPos % boardSize == 0)) {
                    break;
                }
                uint64_t newPosMask = 1ULL << newPos;


                attacks |= newPosMask;

                if ((sameColor & newPosMask) != 0) {
                    break;
                }
                if ((oppositeColor & newPosMask) != 0) {
                    break;
                }
            }
        }
    }
    return attacks;
}

void Board::getStraightMoves(uint64_t pieces, bool white) {
    if (pieces == 0) {
        return;
    }

    uint64_t sameColor = white ? whitePieces : blackPieces;
    uint64_t oppositeColor = white ? blackPieces : whitePieces;


    while (pieces != 0) {
        int startingPosition = __builtin_ctzll(pieces);
        pieces &= pieces - 1;

        for (int dir : straightDirections) {
            int newPos = startingPosition;
            while (true) {
                newPos = newPos + dir;

                if (newPos < 0 || newPos >= numBoardSquares) {
                    break;
                }

                if (newPos >= numBoardSquares) {
                    break;
                }

                if (dir == -1 && (newPos % boardSize == boardSize - 1)) {
                    break;
                }
                if (dir == 1 && (newPos % boardSize == 0)) {
                    break;
                }

                uint64_t newPosMask = 1ULL << newPos;

                if ((sameColor & newPosMask) != 0) {
                    break;
                }

                uint64_t startingPositionMask = 1ULL << startingPosition;

                allPossibleMoves[allPossibleMovesSize].start = startingPositionMask;
                allPossibleMoves[allPossibleMovesSize].end = newPosMask;
                ++allPossibleMovesSize;

                if ((oppositeColor & newPosMask) != 0) {
                    break;
                }
            }
        }
    }
}

uint64_t Board::getRookAttacks(bool white) const {
    uint64_t pieces = white ? pieceBB[whiteRook] : pieceBB[blackRook];

    return getStraightAttacks(pieces, white);
}

void Board::getRookMoves(bool white) {
    uint64_t pieces = white ? pieceBB[whiteRook] : pieceBB[blackRook];

    getStraightMoves(pieces, white);
}

uint64_t Board::getDiagonalAttacks(uint64_t pieces, bool white) const {
    uint64_t sameColor = white ? whitePieces : blackPieces;
    uint64_t oppositeColor = white ? blackPieces : whitePieces;

    uint64_t attacks = 0;

    while (pieces != 0) {
        int startingPosition = __builtin_ctzll(pieces);
        pieces &= pieces - 1;

        for (int dir : diagonalDirections) {
            int newPos = startingPosition;
            while (true) {
                newPos = newPos + dir;

                if (newPos < 0 || newPos >= numBoardSquares) {
                    break;
                }

                if (dir == boardSize + 1 && newPos % boardSize == 0) {
                    break;
                }
                if (dir == boardSize - 1 && newPos % boardSize == boardSize - 1) {
                    break;
                }
                if (dir == -boardSize + 1 && newPos % boardSize == 0) {
                    break;
                }
                if (dir == -boardSize - 1 && newPos % boardSize == boardSize - 1) {
                    break;
                }


                uint64_t newPosMask = 1ULL << newPos;

                attacks |= newPosMask;

                if ((sameColor & newPosMask) != 0) {
                    break;
                }
                if ((oppositeColor & newPosMask) != 0) {
                    break;
                }
            }
        }
        ++startingPosition;
    }
    return attacks;
}

uint64_t Board::getBishopAttacks(bool white) const {
    uint64_t pieces = white ? pieceBB[whiteBishop] : pieceBB[blackBishop];

    return getDiagonalAttacks(pieces, white);
}
uint64_t Board::getQueenAttacks(bool white) const {
    uint64_t pieces = white ? pieceBB[whiteQueen] : pieceBB[blackQueen];

    return getDiagonalAttacks(pieces, white) | getStraightAttacks(pieces, white);
}


void Board::getDiagonalMoves(uint64_t pieces, bool white) {
    uint64_t sameColor = white ? whitePieces : blackPieces;
    uint64_t oppositeColor = white ? blackPieces : whitePieces;

    while (pieces != 0) {
        int startingPosition = __builtin_ctzll(pieces);
        pieces &= pieces - 1;


        for (int dir : diagonalDirections) {
            int newPos = startingPosition;
            while (true) {
                newPos += dir;

                if (newPos < 0 || newPos >= numBoardSquares) {
                    break;
                }
                if (dir == boardSize + 1 && newPos % boardSize == 0) {
                    break;
                }
                if (dir == boardSize - 1 && newPos % boardSize == boardSize - 1) {
                    break;
                }
                if (dir == -(boardSize - 1) && newPos % boardSize == 0) {
                    break;
                }
                if (dir == -(boardSize + 1) && newPos % boardSize == boardSize - 1) {
                    break;
                }

                uint64_t newPosMask = 1ULL << newPos;

                if ((sameColor & newPosMask) != 0) {
                    break;
                }

                uint64_t startingPositionMask = 1ULL << startingPosition;

                allPossibleMoves[allPossibleMovesSize].start = startingPositionMask;
                allPossibleMoves[allPossibleMovesSize].end = newPosMask;
                ++allPossibleMovesSize;
                // Check for opposite-color pieces
                if ((oppositeColor & newPosMask) != 0) {
                    break;
                }
            }
        }
    }
}

void Board::getBishopMoves(bool white) {
    uint64_t pieces = white ? pieceBB[whiteBishop] : pieceBB[blackBishop];

    getDiagonalMoves(pieces, white);
}

void Board::getQueenMoves(bool white) {
    uint64_t pieces = white ? pieceBB[whiteQueen] : pieceBB[blackQueen];
    getStraightMoves(pieces, white);
    getDiagonalMoves(pieces, white);
}

uint64_t Board::getKingAttacks(bool white) const {
    uint64_t attacks = 0;

    uint64_t pieces = white ? pieceBB[whiteKing] : pieceBB[blackKing];

    if (pieces == 0) {
        return attacks;
    }


    int startSquare = __builtin_ctzll(pieces);

    int row = startSquare / boardSize;
    int col = startSquare % boardSize;

    for (int dir : kingDirections) {
        int newPos = startSquare + dir;

        if (newPos < 0 || newPos >= numBoardSquares) {
            continue;
        }
        int rowDiff = abs(row - (newPos / boardSize));
        int colDiff = abs(col - (newPos % boardSize));

        if (rowDiff > 1 || colDiff > 1) {
            continue;
        }

        attacks |= 1ULL << newPos;
    }

    return attacks;
}

void Board::getKingMoves(bool white) {
    uint64_t possibleSpots = ~(white ? whitePieces : blackPieces);

    uint64_t pieces = white ? pieceBB[whiteKing] : pieceBB[blackKing];

    if (pieces == 0) {
        return;
    }

    int startSquare = __builtin_ctzll(pieces);

    int row = startSquare / boardSize;
    int col = startSquare % boardSize;


    for (int dir : kingDirections) {
        int targetSquare = startSquare + dir;

        if (targetSquare < 0 || targetSquare >= numBoardSquares) {
            continue;
        }

        int rowDiff = abs(row - (targetSquare / boardSize));
        int colDiff = abs(col - (targetSquare % boardSize));

        if (rowDiff > 1 || colDiff > 1) {
            continue;
        }

        uint64_t targetMask = (1ULL << targetSquare);

        if ((targetMask & possibleSpots) == 0) {
            continue;
        }

        uint64_t startSquareMask = 1ULL << startSquare;

        allPossibleMoves[allPossibleMovesSize].start = startSquareMask;
        allPossibleMoves[allPossibleMovesSize].end = targetMask;
        ++allPossibleMovesSize;
    }
}

int Board::processMoveWithReEvaulation(const Move& move) {
    int pieceTypeRemoved = -1;
    for (int i = blackPawn; i <= whiteKing; ++i) {
        if ((pieceBB[i] & move.end) != 0) {
            pieceBB[i] &= ~move.end;
            pieceTypeRemoved = i;

            currentEval -= getValueFromPieceType(i, __builtin_ctzll(move.end));

            if (i < whitePawn) {
                blackPieces = (blackPieces & ~move.end);
            } else {
                whitePieces = (whitePieces & ~move.end);
            }
            break;
        }
    }
    for (int i = blackPawn; i <= whiteKing; ++i) {
        if ((pieceBB[i] & move.start) != 0) {
            pieceBB[i] = (pieceBB[i] & ~move.start) | move.end;

            currentEval += getValueFromPieceType(i, __builtin_ctzll(move.end))
                         - getValueFromPieceType(i, __builtin_ctzll(move.start));

            if (i < whitePawn) {
                blackPieces = (blackPieces & ~move.start) | move.end;
            } else {
                whitePieces = (whitePieces & ~move.start) | move.end;
            }
            break;
        }
    }


    whiteTurn = !whiteTurn;
    return pieceTypeRemoved;
}

void Board::unProcessMoveWithReEvaulation(const Move& move, int pieceTypeRemoved) {
    for (int i = blackPawn; i <= whiteKing; ++i) {
        if ((pieceBB[i] & move.end) != 0) {
            pieceBB[i] = (pieceBB[i] & ~move.end) | move.start;

            currentEval += -getValueFromPieceType(i, __builtin_ctzll(move.end))
                         + getValueFromPieceType(i, __builtin_ctzll(move.start));


            if (i < whitePawn) {
                blackPieces = (blackPieces & ~move.end) | move.start;
            } else {
                whitePieces = (whitePieces & ~move.end) | move.start;
            }
            break;
        }
    }
    if (pieceTypeRemoved != -1) {
        pieceBB[pieceTypeRemoved] |= move.end;

        currentEval += getValueFromPieceType(pieceTypeRemoved, __builtin_ctzll(move.end));


        if (pieceTypeRemoved < whitePawn) {
            blackPieces = (blackPieces | move.end);
        } else {
            whitePieces = (whitePieces | move.end);
        }
    }
    whiteTurn = !whiteTurn;
}

int Board::processMove(Move move) {
    int pieceTypeRemoved = -1;
    for (int i = blackPawn; i <= whiteKing; ++i) {
        if ((pieceBB[i] & move.end) != 0) {
            pieceBB[i] &= ~move.end;
            pieceTypeRemoved = i;

            if (i < whitePawn) {
                blackPieces = (blackPieces & ~move.end);
            } else {
                whitePieces = (whitePieces & ~move.end);
            }
            break;
        }
    }
    for (int i = blackPawn; i <= whiteKing; ++i) {
        if ((pieceBB[i] & move.start) != 0) {
            pieceBB[i] = (pieceBB[i] & ~move.start) | move.end;

            if (i < whitePawn) {
                blackPieces = (blackPieces & ~move.start) | move.end;
            } else {
                whitePieces = (whitePieces & ~move.start) | move.end;
            }
            break;
        }
    }

    whiteTurn = !whiteTurn;
    return pieceTypeRemoved;
}

void Board::unProcessMove(Move move, int pieceTypeRemoved) {
    for (int i = blackPawn; i <= whiteKing; ++i) {
        if ((pieceBB[i] & move.end) != 0) {
            pieceBB[i] = (pieceBB[i] & ~move.end) | move.start;

            if (i < whitePawn) {
                blackPieces = (blackPieces & ~move.end) | move.start;
            } else {
                whitePieces = (whitePieces & ~move.end) | move.start;
            }
            break;
        }
    }
    if (pieceTypeRemoved != -1) {
        pieceBB[pieceTypeRemoved] |= move.end;

        if (pieceTypeRemoved < whitePawn) {
            blackPieces = (blackPieces | move.end);
        } else {
            whitePieces = (whitePieces | move.end);
        }
    }
    whiteTurn = !whiteTurn;
}

bool Board::moveIsValidWithCheck(Move move, bool white) {
    int previousValue = processMove(move);

    uint64_t oppositeAttacks = getPawnAttacks(!white) | getKnightAttacks(!white) | getBishopAttacks(!white)
                             | getRookAttacks(!white) | getQueenAttacks(!white) | getKingAttacks(!white);

    bool valid = (oppositeAttacks & (white ? pieceBB[whiteKing] : pieceBB[blackKing])) == 0;

    unProcessMove(move, previousValue);

    return valid;
}

void Board::addValidMoves(const std::vector<Move>& potentialMoves, std::vector<Move>& moves, bool white) {
    for (const Move& move : potentialMoves) {
        if (moveIsValidWithCheck(move, white)) {
            moves.emplace_back(move);
        }
    }
}

void Board::getValidMovesNoCheckNoKing(bool white) {
    vector<Move> moves;

    getPawnMoves(white);
    getKnightMoves(white);
    getBishopMoves(white);
    getRookMoves(white);
    getQueenMoves(white);
}

std::vector<Move> Board::getValidMovesWithCheck() {
    allPossibleMovesSize = 0;


    uint64_t kingMask = whiteTurn ? pieceBB[whiteKing] : pieceBB[blackKing];
    int kingPosition = __builtin_ctzll(kingMask);

    uint64_t sameColor = whiteTurn ? whitePieces : blackPieces;
    uint64_t oppositeColor = whiteTurn ? blackPieces : whitePieces;

    uint64_t oppositeColorDiagonalPieces
      = whiteTurn ? pieceBB[blackBishop] | pieceBB[blackQueen] : pieceBB[whiteBishop] | pieceBB[whiteQueen];

    uint64_t oppositeColorStraightPieces
      = whiteTurn ? pieceBB[blackRook] | pieceBB[blackQueen] : pieceBB[whiteRook] | pieceBB[whiteQueen];


    uint64_t slidingAttacksMask = 0;
    uint64_t nonSlidingAttacksMask = 0;

    if (whiteTurn) {
        slidingAttacksMask = getBishopAttacks(false) | getRookAttacks(false) | getQueenAttacks(false);
        nonSlidingAttacksMask = getPawnAttacks(false) | getKnightAttacks(false) | getKingAttacks(false);
    } else {
        slidingAttacksMask = getBishopAttacks(true) | getRookAttacks(true) | getQueenAttacks(true);
        nonSlidingAttacksMask = getPawnAttacks(true) | getKnightAttacks(true) | getKingAttacks(true);
    }

    uint64_t combinedAttacks = slidingAttacksMask | nonSlidingAttacksMask;

    bool doubleSlidingAttack = false;
    int slidingAttackDirection = 0;
    int slidingAttackDirection2 = 0;
    int slidingPieceAttackerLocation = -1;
    int slidingPieceAttackerLocation2 = -1;


    int kingRow = kingPosition / boardSize;
    int kingCol = kingPosition % boardSize;

    bool kingUnderAttackBySlidingPiece = (kingMask & slidingAttacksMask) != 0;

    uint64_t pinnedPieces = 0;

    for (int dir : straightDirections) {
        int newPos = kingPosition;

        int closestSameColorPieceLocation = -1;

        while (true) {
            int lastRow = newPos / boardSize;
            int lastCol = newPos % boardSize;
            newPos = newPos + dir;

            if (newPos < 0 || newPos >= numBoardSquares) {
                break;
            }

            int rowDiff = abs(lastRow - (newPos / boardSize));
            int colDiff = abs(lastCol - (newPos % boardSize));

            if (rowDiff > 1 || colDiff > 1) {
                break;
            }

            uint64_t newPosMask = 1ULL << newPos;

            if (kingUnderAttackBySlidingPiece && closestSameColorPieceLocation == -1
                && (oppositeColorStraightPieces & newPosMask) != 0) {
                if (slidingPieceAttackerLocation != -1) {
                    doubleSlidingAttack = true;
                    slidingAttackDirection2 = dir;
                    slidingPieceAttackerLocation2 = newPos;
                    break;
                }
                slidingAttackDirection = dir;
                slidingPieceAttackerLocation = newPos;
                break;
            }

            if ((sameColor & newPosMask) != 0) {
                if (closestSameColorPieceLocation != -1) {
                    break;
                }
                closestSameColorPieceLocation = newPos;
            }

            if ((oppositeColorStraightPieces & newPosMask) != 0) {
                pinnedPieces |= (1ULL << closestSameColorPieceLocation);
                break;
            }

            if ((oppositeColor & newPosMask) != 0) {
                break;
            }
        }
    }

    for (int dir : diagonalDirections) {
        int newPos = kingPosition;

        int closestSameColorPieceLocation = -1;

        while (true) {
            int lastRow = newPos / boardSize;
            int lastCol = newPos % boardSize;

            newPos = newPos + dir;

            if (newPos < 0 || newPos >= numBoardSquares) {
                break;
            }

            int rowDiff = abs(lastRow - (newPos / boardSize));
            int colDiff = abs(lastCol - (newPos % boardSize));

            if (rowDiff > 1 || colDiff > 1) {
                break;
            }


            uint64_t newPosMask = 1ULL << newPos;

            if (kingUnderAttackBySlidingPiece && closestSameColorPieceLocation == -1
                && (oppositeColorDiagonalPieces & newPosMask) != 0) {
                if (slidingPieceAttackerLocation != -1) {
                    doubleSlidingAttack = true;
                    slidingAttackDirection2 = dir;
                    slidingPieceAttackerLocation2 = newPos;
                    break;
                }
                slidingAttackDirection = dir;
                slidingPieceAttackerLocation = newPos;
                break;
            }

            if ((sameColor & newPosMask) != 0) {
                if (closestSameColorPieceLocation != -1) {
                    break;
                }
                closestSameColorPieceLocation = newPos;
            }

            if ((oppositeColorDiagonalPieces & newPosMask) != 0) {
                pinnedPieces |= (1ULL << closestSameColorPieceLocation);
                break;
            }

            if ((oppositeColor & newPosMask) != 0) {
                break;
            }
        }
    }


    vector<Move> moves;


    if (doubleSlidingAttack || ((kingMask & slidingAttacksMask) != 0 && (kingMask & nonSlidingAttacksMask) != 0)) {
        // currently attacked by 2 sliding pieces or a sliding piece and non sliding piece
        // only possible move is to have the king move out of the way

        for (int dir : kingDirections) {
            int newPos = kingPosition + dir;

            if (newPos < 0 || newPos >= numBoardSquares) {
                continue;
            }
            if (dir == slidingAttackDirection && newPos != slidingPieceAttackerLocation) {
                continue;
            }
            if (slidingPieceAttackerLocation2 != -1 && dir == slidingAttackDirection2
                && newPos != slidingPieceAttackerLocation2) {
                continue;
            }
            if (dir == -slidingAttackDirection) {
                continue;
            }
            if (slidingAttackDirection2 != -1 && dir == -slidingAttackDirection2) {
                continue;
            }

            int newRow = newPos / boardSize;
            int newCol = newPos % boardSize;

            int rowDiff = abs(newRow - kingRow);
            int colDiff = abs(newCol - kingCol);

            if (rowDiff > 1 || colDiff > 1) {
                continue;
            }

            uint64_t newPosMask = 1ULL << newPos;

            if ((newPosMask & combinedAttacks) != 0) {
                continue;
            }
            if ((newPosMask & sameColor) != 0) {
                continue;
            }
            moves.emplace_back(kingMask, newPosMask);
        }

        return moves;
    }

    if (kingUnderAttackBySlidingPiece) {
        // currently under attack by 1 sliding piece
        // need to block, capture attacker, or move out of the way
        // not possible for pinned pieces to stop the attack
        // pinned pieces can not move
        for (int dir : kingDirections) {
            int newPos = kingPosition + dir;

            if ((dir == slidingAttackDirection || dir == -slidingAttackDirection)
                && newPos != slidingPieceAttackerLocation) {
                continue;
            }
            if (newPos < 0 || newPos >= numBoardSquares) {
                continue;
            }

            int newRow = newPos / boardSize;
            int newCol = newPos % boardSize;
            int rowDiff = abs(newRow - kingRow);
            int colDiff = abs(newCol - kingCol);

            if (rowDiff > 1 || colDiff > 1) {
                continue;
            }
            uint64_t newPosMask = 1ULL << newPos;
            if ((newPosMask & combinedAttacks) != 0) {
                continue;
            }
            if ((newPosMask & sameColor) != 0) {
                continue;
            }
            moves.emplace_back(kingMask, newPosMask);
        }


        vector<uint64_t> validEndingSpots;

        for (int i = kingPosition; i != slidingPieceAttackerLocation; i += slidingAttackDirection) {
            validEndingSpots.emplace_back(1ULL << i);
        }
        validEndingSpots.emplace_back(1ULL << slidingPieceAttackerLocation);


        allPossibleMovesSize = 0;
        getValidMovesNoCheckNoKing(whiteTurn);

        for (int i = 0; i < allPossibleMovesSize; ++i) {
            const Move& move = allPossibleMoves[i];

            if ((move.start & pinnedPieces) != 0) {
                continue;
            }

            for (uint64_t validEndingSpot : validEndingSpots) {
                if ((move.end & validEndingSpot) != 0) {
                    moves.emplace_back(move);
                    break;
                }
            }
        }
        return moves;
    }

    if ((kingMask & nonSlidingAttacksMask) != 0) {
        // currently attacked by pawn or by knight
        // can only be attacked by 1 piece
        // need to capture attacker or move out of the way
        // not possible for pinned pieces to stop the attack
        // pinned pieces can not move

        allPossibleMovesSize = 0;
        getKingMoves(whiteTurn);

        for (int i = 0; i < allPossibleMovesSize; ++i) {
            const Move& move = allPossibleMoves[i];
            if ((move.end & combinedAttacks) == 0) {
                // if king move ends on a non attacked square it is a valid move
                moves.emplace_back(move);
            }
        }


        uint64_t attackSquareMask = 0;

        uint64_t pawnAttacks = getPawnAttacks(!whiteTurn);

        if ((pawnAttacks & kingMask) != 0) {
            // king is being attacked by a pawn
            uint64_t pawns = whiteTurn ? pieceBB[blackPawn] : pieceBB[whitePawn];

            if (kingCol < boardSize) {
                // left attack is possible
                int pawnAttackingLeft = whiteTurn ? kingPosition - boardSize + 1 : kingPosition + boardSize + 1;

                uint64_t pawnAttackingLeftMask = 1ULL << pawnAttackingLeft;

                if ((pawns & pawnAttackingLeftMask) != 0) {
                    attackSquareMask = pawnAttackingLeftMask;
                }
            }
            if (attackSquareMask == 0) {
                int rightAttackPosition = whiteTurn ? kingPosition - boardSize - 1 : kingPosition + boardSize - 1;

                attackSquareMask = 1ULL << rightAttackPosition;
            }
        } else {
            uint64_t knights = whiteTurn ? pieceBB[blackKnight] : pieceBB[whiteKnight];


            for (int dir : knightOffsets) {
                int newPos = kingPosition + dir;

                if (newPos < 0 || newPos >= numBoardSquares) {
                    continue;
                }

                int newRow = newPos / boardSize;
                int newCol = newPos % boardSize;

                int rowDiff = abs(newRow - kingRow);
                int colDiff = abs(newCol - kingCol);

                if ((rowDiff != 2 || colDiff != 1) && (rowDiff != 1 || colDiff != 2)) {
                    continue;
                }


                uint64_t newPosMask = 1ULL << newPos;

                if ((knights & newPosMask) != 0) {
                    attackSquareMask = newPosMask;
                    break;
                }
            }
        }

        allPossibleMovesSize = 0;
        getValidMovesNoCheckNoKing(whiteTurn);

        for (int i = 0; i < allPossibleMovesSize; ++i) {
            const Move& move = allPossibleMoves[i];
            if (move.end == attackSquareMask && (move.start & pinnedPieces) == 0) {
                moves.emplace_back(move);
            }
        }
        return moves;
    }
    // currently not in check
    // only need to check if moving reveals a sliding attack
    // mark pinned pieces and do not let pinned pieces move off attack line

    allPossibleMovesSize = 0;
    getKingMoves(whiteTurn);

    for (int i = 0; i < allPossibleMovesSize; ++i) {
        const Move& move = allPossibleMoves[i];
        if ((move.end & combinedAttacks) == 0) {
            // if king move ends on a non attacked square it is a valid move
            moves.emplace_back(move);
        }
    }

    allPossibleMovesSize = 0;
    getValidMovesNoCheckNoKing(whiteTurn);

    for (int i = 0; i < allPossibleMovesSize; ++i) {
        const Move& move = allPossibleMoves[i];

        if ((move.start & pinnedPieces) != 0) {
            int startPos = __builtin_ctzll(move.start);
            int endPos = __builtin_ctzll(move.end);

            int startRow = startPos / boardSize;

            if (startRow == kingRow) {
                if (startRow != endPos / boardSize) {
                    continue;
                }
                moves.emplace_back(move);
                continue;
            }
            int startCol = startPos % boardSize;

            if (startCol == kingCol) {
                if (startCol != endPos % boardSize) {
                    continue;
                }
                moves.emplace_back(move);
                continue;
            }

            int endRow = endPos / boardSize;
            int endCol = endPos % boardSize;

            if (startRow < kingRow && endRow >= kingRow) {
                continue;
            }
            if (startRow > kingRow && endRow <= kingRow) {
                continue;
            }
            if (startCol < kingCol && endCol >= kingCol) {
                continue;
            }
            if (startCol > kingCol && endCol <= kingCol) {
                continue;
            }
            if (abs(startRow - kingRow) != abs(endRow - kingRow) || abs(startCol - kingCol) != abs(endCol - kingCol)) {
                continue;
            }
            moves.emplace_back(move);

        } else {
            moves.emplace_back(move);
        }
    }

    return moves;
}

double Board::evaluation() const {
    return currentEval;
}

pair<Move, bool> Board::processUserInput(const string& userInput) {
    size_t numChars = userInput.size();

    int rowEnd = boardSize - (userInput[numChars - 1] - '0');
    int colEnd = userInput[numChars - 2] - 'a';

    int endPosition = rowEnd * boardSize + colEnd;
    uint64_t endPositionMask = 1ULL << endPosition;

    allPossibleMovesSize = 0;

    if (userInput[0] >= 'a' && userInput[0] <= 'h') {
        getPawnMoves(whiteTurn);

        if (userInput.size() == 2) {
            for (int i = 0; i < allPossibleMovesSize; ++i) {
                const Move& move = allPossibleMoves[i];

                if (move.end != endPositionMask) {
                    continue;
                }
                int position = __builtin_ctzll(move.start);

                if (position % boardSize != colEnd) {
                    continue;
                }

                return { move, true };
            }
        } else {
            int colStart = userInput[0] - 'a';

            for (int i = 0; i < allPossibleMovesSize; ++i) {
                const Move& move = allPossibleMoves[i];


                if (move.end != endPositionMask) {
                    continue;
                }
                int position = __builtin_ctzll(move.start);

                if (position % boardSize != colStart) {
                    continue;
                }

                return { move, true };
            }
        }
    }

    int rowStart = -1;
    int colStart = -1;

    for (size_t i = 1; i < userInput.size() - 2; ++i) {
        if (userInput[i] == 'x') {
            continue;
        }
        if (isdigit(userInput[i]) != 0) {
            rowStart = 8 - (userInput[i] - '0');
        } else {
            colStart = userInput[i] - 'a';
        }
    }

    switch (userInput[0]) {
    case 'N':
        getKnightMoves(whiteTurn);
        break;
    case 'B':
        getBishopMoves(whiteTurn);
        break;
    case 'R':
        getRookMoves(whiteTurn);
        break;
    case 'Q':
        getQueenMoves(whiteTurn);
        break;
    case 'K':
        getKingMoves(whiteTurn);
    default:
        break;
    }

    for (int i = 0; i < allPossibleMovesSize; ++i) {
        const Move& move = allPossibleMoves[i];

        if (move.end != endPositionMask) {
            continue;
        }
        int position = __builtin_ctzll(move.start);
        if (rowStart != -1 && rowStart != position / boardSize) {
            continue;
        }
        if (colStart != -1 && colStart != position % boardSize) {
            continue;
        }
        return { move, true };
    }
    return { Move(), false };
}


void Board::displayBoard() const {
    vector<std::vector<char>> boardCharacters(boardSize, vector<char>(boardSize, '.'));

    for (int i = 0; i < numTypesPieces; ++i) {
        uint64_t pieces = pieceBB[i];


        while (pieces != 0) {
            int startSquare = __builtin_ctzll(pieces);
            pieces &= pieces - 1;

            boardCharacters[startSquare / boardSize][startSquare % boardSize] = getCharFromPieceType(i);
        }
    }

    for (int r = 0; r < boardSize; ++r) {
        cout << boardSize - r << " ";

        for (int c = 0; c < boardSize; ++c) {
            cout << boardCharacters[r][c] << " ";
        }
        cout << "\n";
    }

    cout << "  ";
    for (char i = 'a'; i <= 'h'; ++i) {
        cout << i << " ";
    }
    cout << "\n";
}

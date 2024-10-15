#include "Board.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "Constants.hpp"
#include "Move.hpp"

using namespace std;

Board::Board(const string& fen, std::array<uint64_t, numBoardSquares>& knightMoves)
    : knightMoves(knightMoves) {
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
};

uint64_t Board::getWhitePawnAttacks() const {
    return (pieceBB[whitePawn] >> (boardSize - 1) & pawnAttackingRight)
         | (pieceBB[whitePawn] >> (boardSize + 1) & pawnAttackingLeft);
}

vector<Move> Board::getPawnMoves(bool white) const {
    uint64_t emptySquares = ~(whitePieces | blackPieces);

    vector<Move> moves;

    if (white) {
        uint64_t pawns = pieceBB[whitePawn];

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
                moves.emplace_back(startSquareMask, target);
            }

            // Double step forward move
            target = startSquareMask >> boardSize * 2;
            if ((doubleStep & target) != 0) {
                moves.emplace_back(startSquareMask, target);
            }

            // Attack left
            target = startSquareMask >> (boardSize - 1);
            if ((attackLeft & target) != 0) {
                moves.emplace_back(startSquareMask, target);
            }

            // Attack right
            target = startSquareMask >> (boardSize + 1);
            if ((attackRight & target) != 0) {
                moves.emplace_back(startSquareMask, target);
            }
        }
    } else {
        uint64_t pawns = pieceBB[blackPawn];

        uint64_t startRow = rowMasks(1);

        uint64_t singleStep = (pawns >> boardSize) & emptySquares;
        uint64_t doubleStep = ((pawns & startRow) >> (boardSize * 2)) & emptySquares;
        uint64_t attackLeft = (pawns >> (boardSize + 1)) & pawnAttackingRight & whitePieces;
        uint64_t attackRight = (pawns >> (boardSize - 1)) & pawnAttackingLeft & whitePieces;

        while (pawns != 0) {
            int startSquare = __builtin_ctzll(pawns);
            pawns &= pawns - 1;

            uint64_t startSquareMask = 1ULL << startSquare;

            // Single step forward move
            uint64_t target = startSquareMask << boardSize;
            if ((singleStep & target) != 0) {
                moves.emplace_back(startSquareMask, target);
            }

            // Double step forward move
            target = startSquareMask << (boardSize * 2);
            if ((doubleStep & target) != 0) {
                moves.emplace_back(startSquareMask, target);
            }

            // Attack left
            target = startSquareMask << (boardSize + 1);
            if ((attackLeft & target) != 0) {
                moves.emplace_back(startSquareMask, target);
            }

            // Attack right
            target = startSquareMask << (boardSize - 1);
            if ((attackRight & target) != 0) {
                moves.emplace_back(startSquareMask, target);
            }
        }
    }
    return moves;
}

uint64_t Board::getBlackPawnAttacks() const {
    return (pieceBB[blackPawn] << (boardSize - 1) & pawnAttackingLeft)
         | (pieceBB[blackPawn] << (boardSize + 1) & pawnAttackingRight);
}

uint64_t Board::getKnightAttacks(bool white) const {
    uint64_t knights = white ? pieceBB[whiteKnight] : pieceBB[blackKnight];

    uint64_t attacks = 0;

    while (knights != 0) {
        int startSquare = __builtin_ctzll(knights);
        knights &= knights - 1;

        attacks |= Board::knightMoves[startSquare];
    }

    return attacks;
}

vector<Move> Board::getKnightMoves(bool white) const {
    uint64_t knights = white ? pieceBB[whiteKnight] : pieceBB[blackKnight];

    uint64_t possibleSpots = ~(white ? whitePieces : blackPieces);

    vector<Move> moves;

    while (knights != 0) {
        int startSquare = __builtin_ctzll(knights);
        knights &= knights - 1;

        for (int offset : knightOffsets) {
            int targetSquare = startSquare + offset;

            int row = startSquare / boardSize;
            int col = startSquare % boardSize;

            int newRow = targetSquare / boardSize;
            int newCol = targetSquare % boardSize;

            int rowDiff = abs(newRow - row);
            int colDiff = abs(newCol - col);

            if ((rowDiff != 2 && colDiff != 1) && (rowDiff != 1 || colDiff != 2)) {
                continue;
            }


            uint64_t targetMask = (1ULL << targetSquare);

            if ((targetMask & possibleSpots) == 0) {
                continue;
            }

            uint64_t startSquareMask = 1ULL << startSquare;

            moves.emplace_back(startSquareMask, targetMask);
        }
    }
    return moves;
}


uint64_t Board::getStraightAttacks(uint64_t pieces, bool white) const {
    uint64_t sameColor = white ? whitePieces : blackPieces;
    uint64_t oppositeColor = white ? blackPieces : whitePieces;

    uint64_t attacks = 0;

    while (pieces > 0) {
        int startingPosition = __builtin_ctzll(pieces);
        pieces &= pieces - 1;

        for (int dir : straightDirections) {
            int newPos = startingPosition;
            while (true) {
                newPos = newPos + dir;

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

std::vector<Move> Board::getStraightMoves(uint64_t pieces, bool white) const {
    uint64_t sameColor = white ? whitePieces : blackPieces;
    uint64_t oppositeColor = white ? blackPieces : whitePieces;

    vector<Move> moves;

    uint64_t startingPositionMask = 1;

    while (pieces > 0) {
        int startingPosition = __builtin_ctzll(pieces);
        pieces &= pieces - 1;

        if ((pieces & 1) == 0) {
            pieces >>= 1;
            startingPositionMask <<= 1;
            ++startingPosition;
            continue;
        }
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

                if ((sameColor & newPosMask) == 1) {
                    break;
                }
                moves.emplace_back(startingPositionMask, newPosMask);

                if ((oppositeColor & startingPositionMask) == 1) {
                    break;
                }
            }
        }
        pieces >>= 1;
        startingPositionMask <<= 1;
        ++startingPosition;
    }
    return moves;
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
                if (dir == -(boardSize - 1) && newPos % boardSize == 0) {
                    break;
                }
                if (dir == -(boardSize + 1) && newPos % boardSize == boardSize - 1) {
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

std::vector<Move> Board::getDiagonalMoves(uint64_t pieces, bool white) const {
    uint64_t sameColor = white ? whitePieces : blackPieces;
    uint64_t oppositeColor = white ? blackPieces : whitePieces;

    std::vector<Move> moves;

    while (pieces != 0) {
        int startingPosition = __builtin_ctzll(pieces);
        pieces &= pieces - 1;


        uint64_t startingPositionMask = 1ULL << startingPosition;


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

                if ((sameColor & newPosMask) == 1) {
                    break;
                }

                moves.emplace_back(startingPositionMask, newPosMask);

                // Check for opposite-color pieces
                if ((oppositeColor & newPosMask) == 1) {
                    break;
                }
            }
        }
    }
    return moves;
}

vector<Move> Board::getQueenMoves(bool white) const {
    uint64_t pieces = white ? pieceBB[whiteQueen] : pieceBB[blackQueen];
    vector<Move> straightMoves = getStraightMoves(pieces, white);


    vector<Move> diagonalMoves = getDiagonalMoves(pieces, white);


    vector<Move> queenMoves;
    queenMoves.reserve(straightMoves.size() + diagonalMoves.size());

    queenMoves.insert(queenMoves.end(), straightMoves.begin(), straightMoves.end());
    queenMoves.insert(queenMoves.end(), diagonalMoves.begin(), diagonalMoves.end());

    return queenMoves;
}

void Board::processMove(Move move) {
    for (int i = blackPawn; i < whiteKing; ++i) {
        if ((pieceBB[i] & move.start) == 1) {
            pieceBB[i] = (pieceBB[i] & ~move.start) | move.end;

            if (i < whitePawn) {
                blackPieces &= (blackPieces & ~move.start) | move.end;
            } else {
                whitePieces &= (whitePieces & ~move.start) | move.end;
            }


            break;
        }
    }
}

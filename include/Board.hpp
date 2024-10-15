#ifndef GAMEBOARD_h
#define GAMEBOARD_h

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "Constants.hpp"
#include "Move.hpp"

class Board {
private:
    uint64_t pieceBB[12];

    uint64_t whitePieces = 0;
    uint64_t blackPieces = 0;

    // Bitboards for white attacks
    uint64_t whiteSlidingAttacking = 0;
    uint64_t whiteNonSlidingAttacking = 0;

    // Bitboards for black attacks
    uint64_t blackNonSlidingAttacking = 0;
    uint64_t blackSlidingAttacking = 0;

    int castleRights = 0b1111;

    std::vector<Move> moves;

    std::array<uint64_t, numBoardSquares>& knightMoves;

    std::array<uint64_t, numBoardSquares> generateKnightMoves();

public:
    Board(std::array<uint64_t, numBoardSquares>& knightMoves)
        : Board(defaultBoardPosition, knightMoves) {}
    Board(const std::string& fen, std::array<uint64_t, numBoardSquares>&);
    Board(const Board& board) = default;

    uint64_t getWhitePawnAttacks() const;
    uint64_t getBlackPawnAttacks() const;
    uint64_t getKnightAttacks(bool white) const;
    uint64_t getStraightAttacks(uint64_t pieces, bool white) const;
    uint64_t getDiagonalAttacks(uint64_t pieces, bool white) const;

    std::vector<Move> getPawnMoves(bool white) const;
    std::vector<Move> getKnightMoves(bool white) const;
    std::vector<Move> getStraightMoves(uint64_t pieces, bool white) const;
    std::vector<Move> getDiagonalMoves(uint64_t pieces, bool white) const;
    std::vector<Move> getQueenMoves(bool white) const;
    std::vector<Move> getKingMoves(bool white) const;

    void processMove(Move move);
};

#endif

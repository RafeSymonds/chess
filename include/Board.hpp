#ifndef GAMEBOARD_h
#define GAMEBOARD_h

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


public:
    Board()
        : Board(defaultBoardPosition) {}
    Board(const std::string& fen);
    Board(const Board& board) = default;

    uint64_t getWhitePawnAttacks();
    uint64_t getBlackPawnAttacks();
    uint64_t getKnightAttacks(bool white);
    uint64_t getRookAttacks(bool white);
    std::vector<Move> getPawnMoves(int position) const;
    std::vector<Move> getRookMoves(int position) const;
    std::vector<Move> getKnightMoves(int position) const;
    std::vector<Move> getBishopMoves(int position) const;
    std::vector<Move> getQueenMoves(int position) const;
    std::vector<Move> getKingMoves(int position) const;
};

#endif

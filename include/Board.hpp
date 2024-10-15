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
    std::array<uint64_t, numTypesPieces> pieceBB {};

    uint64_t whitePieces = 0;
    uint64_t blackPieces = 0;

    // Bitboards for white attacks
    uint64_t whiteSlidingAttacking = 0;
    uint64_t whiteNonSlidingAttacking = 0;

    // Bitboards for black attacks
    uint64_t blackNonSlidingAttacking = 0;
    uint64_t blackSlidingAttacking = 0;

    int castleRights = 0b1111;

    bool whiteTurn = true;

    std::vector<Move> moves;

    std::array<uint64_t, numBoardSquares>* knightMoves;

    std::vector<Move> getStraightMoves(uint64_t pieces, bool white) const;
    std::vector<Move> getDiagonalMoves(uint64_t pieces, bool white) const;


public:
    Board(std::array<uint64_t, numBoardSquares>* knightMoves)
        : Board(defaultBoardPosition, knightMoves) {}
    Board(const std::string& fen, std::array<uint64_t, numBoardSquares>* knightMoves);

    Board(const Board& other) = default;

    Board& operator=(const Board& other) {
        if (this != &other) {
            pieceBB = other.pieceBB;
            whitePieces = other.whitePieces;
            blackPieces = other.blackPieces;
            whiteSlidingAttacking = other.whiteSlidingAttacking;
            whiteNonSlidingAttacking = other.whiteNonSlidingAttacking;
            blackNonSlidingAttacking = other.blackNonSlidingAttacking;
            blackSlidingAttacking = other.blackSlidingAttacking;
            castleRights = other.castleRights;
            whiteTurn = other.whiteTurn;
            moves = other.moves;
        }

        return *this;
    }

    bool isWhiteTurn() const { return whiteTurn; }

    uint64_t getPawnAttacks(bool white) const;

    uint64_t getKnightAttacks(bool white) const;

    uint64_t getStraightAttacks(uint64_t pieces, bool white) const;
    uint64_t getRookAttacks(bool white) const;

    uint64_t getDiagonalAttacks(uint64_t pieces, bool white) const;
    uint64_t getBishopAttacks(bool white) const;
    uint64_t getQueenAttacks(bool white) const;
    uint64_t getKingAttacks(bool white) const;

    std::vector<Move> getPawnMoves(bool white) const;
    std::vector<Move> getKnightMoves(bool white) const;

    std::vector<Move> getBishopMoves(bool white) const;
    std::vector<Move> getRookMoves(bool white) const;
    std::vector<Move> getQueenMoves(bool white) const;
    std::vector<Move> getKingMoves(bool white) const;

    int processMove(Move move);
    void unProcessMove(Move move, int pieceTypeRemoved);

    bool moveIsValidWithCheck(Move move, bool white);

    void addValidMoves(const std::vector<Move>& potentialMoves, std::vector<Move>& moves, bool white);

    std::vector<Move> getValidMovesWithCheck();

    double evaluation() const;

    std::pair<Move, bool> processUserInput(const std::string& userInput) const;

    void displayBoard() const;
};

#endif

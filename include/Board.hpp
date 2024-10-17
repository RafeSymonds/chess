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

    double currentEval = 0;

    bool gameOver = false;

    std::vector<Move> allPossibleMoves;
    int allPossibleMovesSize = 0;

    std::array<uint64_t, numBoardSquares>* knightMoves;

    void getStraightMoves(uint64_t pieces, bool white);
    void getDiagonalMoves(uint64_t pieces, bool white);


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
            allPossibleMoves = other.allPossibleMoves;
        }

        return *this;
    }


    bool isWhiteTurn() const { return whiteTurn; }

    bool isGameOver() const { return gameOver; }
    void setGameOver() { gameOver = true; }

    uint64_t getPawnAttacks(bool white) const;

    uint64_t getKnightAttacks(bool white) const;

    uint64_t getStraightAttacks(uint64_t pieces, bool white) const;
    uint64_t getRookAttacks(bool white) const;

    uint64_t getDiagonalAttacks(uint64_t pieces, bool white) const;
    uint64_t getBishopAttacks(bool white) const;
    uint64_t getQueenAttacks(bool white) const;
    uint64_t getKingAttacks(bool white) const;

    void getPawnMoves(bool white);
    void getKnightMoves(bool white);

    void getBishopMoves(bool white);
    void getRookMoves(bool white);
    void getQueenMoves(bool white);
    void getKingMoves(bool white);

    int processMoveWithReEvaulation(const Move& move);
    void unProcessMoveWithReEvaulation(const Move& move, int pieceTypeRemoved);

    int processMove(Move move);
    void unProcessMove(Move move, int pieceTypeRemoved);

    bool moveIsValidWithCheck(Move move, bool white);

    void addValidMoves(const std::vector<Move>& potentialMoves, std::vector<Move>& moves, bool white);

    void getValidMovesNoCheckNoKing(bool white);

    std::vector<Move> getValidMovesWithCheck();

    double evaluation() const;

    std::pair<Move, bool> processUserInput(const std::string& userInput);

    void displayBoard() const;


    void resetAllPossibleMoves() { allPossibleMovesSize = 0; }
};

#endif

#ifndef GAMEBOARD_h
#define GAMEBOARD_h

#include <string>
#include <vector>

#include "Constants.hpp"
#include "Move.hpp"
#include "Position.hpp"

class Board {
private:
    std::vector<std::vector<PieceTypes>> board
      = std::vector<std::vector<PieceTypes>>(boardSize, std::vector<PieceTypes>(boardSize));
    bool whiteShort = true;
    bool whiteLong = true;
    bool blackShort = true;
    bool blackLong = true;
    bool whiteTurn = true;

    std::vector<Move> generateMoves() const;

    bool gameOver = false;

    Position whiteKingPosition;
    Position blackKingPosition;

    inline PieceTypes getPieceType(const Position& position) const;
    inline PieceTypes& getPieceType(const Position& position);

    bool knightAttacks(const Position& position) const;
    bool straightAttacks(const Position& position) const;
    bool diagonalAttacks(const Position& position) const;

    bool kingInCheck(const Position& position) const;

    bool isValidMoveForPawn(const Move& move, PieceTypes pieceType) const;
    bool isValidMoveForKnight(const Move& move) const;
    bool isValidMoveForRook(const Move& move) const;

    bool isValidMoveForBishop(const Move& move) const;
    bool isValidMoveForQueen(const Move& move) const;
    bool isValidMoveForKing(const Move& move) const;

public:
    Board()
        : Board(defaultBoardPosition) {}
    Board(const std::string& fen);

    bool validMoveNoCheck(const Move& move) const;
    bool validMoveWithCheck(const Move& move);

    void processMove(const Move& move);

    void displayBoard() const;

    bool gameIsOver() const;
};

#endif
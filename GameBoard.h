#ifndef GAMEBOARD_h
#define GAMEBOARD_h
#include <iostream>
#include <string>
#include <vector>

#include "Constants.h"
#include "Move.h"

class GameBoard {
private:
    std::vector<std::vector<Pieces>> board
      = std::vector<std::vector<Pieces>>(BOARD_SIZE, std::vector<Pieces>(BOARD_SIZE));
    bool whiteShort = true;
    bool whiteLong = true;
    bool blackShort = true;
    bool blackLong = true;
    bool whiteTurn = true;

    std::vector<Move> generateMoves() const;

    bool gameOver = false;

    Position whiteKingPosition;
    Position blackKingPosition;

public:
    GameBoard()
        : GameBoard(BOARD_DEFAULT_POSITION) {}
    GameBoard(const std::string& fen);

    bool gameIsOver() const;

    void processMove(Move& move);

    static bool validPosition(int row, int column);

    bool validMove(Move& move);

    bool kingInCheck(Move& move);


    Pieces& getPieceFromPosition(Position& position);

    bool bishopMoveCheck(Move& move);
    bool rookMoveCheck(Move& move);
};

#endif

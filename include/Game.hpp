#ifndef GAME_H
#define GAME_H

#include "Board.hpp"
#include "Constants.hpp"
#include "Engine.hpp"

class Game {
private:
    Board currentBoard;
    Engine engine;

public:
    Game(std::array<uint64_t, numBoardSquares>& knightMoves)
        : Game(1, defaultBoardPosition, 3, knightMoves) {};

    Game(int threadNum, const std::string& fen, int depth, std::array<uint64_t, numBoardSquares>& knightMoves)
        : currentBoard(fen, knightMoves)
        , engine(threadNum, currentBoard, depth) {};

    void runGame();

    std::pair<Move, bool> processUserInput(std::string& userInput);
};

#endif

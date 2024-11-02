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
    Game(std::array<uint64_t, numBoardSquares>* knightMoves, BoardHashing& boardHashing)
        : Game(1, defaultBoardPosition, 3, knightMoves, boardHashing) {};

    Game(int threadNum, const std::string& fen, int depth, std::array<uint64_t, numBoardSquares>* knightMoves,
         BoardHashing& boardHashing)
        : currentBoard(fen, knightMoves, boardHashing)
        , engine(threadNum, currentBoard, depth) {};

    void runGame();

    std::pair<Move, bool> processUserInput(std::string& userInput);
};

#endif

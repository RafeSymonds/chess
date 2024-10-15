#ifndef GAME_H
#define GAME_H

#include "Board.hpp"
#include "Engine.hpp"

class Game {
private:
    Board currentBoard;
    Engine engine;

public:
    Game()
        : engine(1, currentBoard, 3) {};

    Game(int threadNum, const std::string& fen, int depth)
        : currentBoard(fen)
        , engine(threadNum, currentBoard, depth) {};

    void runGame();

    std::pair<Move, bool> processUserInput(std::string& userInput);
};

#endif

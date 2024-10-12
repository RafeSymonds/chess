#ifndef GAME_H
#define GAME_H

#include "Board.hpp"

class Game {
private:
    Board currentBoard;

public:
    void runGame();

    std::pair<Move, bool> processUserInput(std::string& userInput);
};

#endif

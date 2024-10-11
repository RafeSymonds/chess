#ifndef GAME_H
#define GAME_H

#include "GameBoard.h"

class Game {
private:
    GameBoard currentBoard;

public:
    void runGame();

    std::pair<Move, bool> processUserInput(std::string& userInput);
};

#endif

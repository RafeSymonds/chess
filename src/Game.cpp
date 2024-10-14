#include "Game.hpp"

#include <cstddef>
#include <iostream>

#include "Constants.hpp"
#include "Position.hpp"


using namespace std;

void Game::runGame() {
    Board& board = engine.getBoard();

    while (!board.gameIsOver()) {
        board.displayBoard();

        string userInput;

        cin >> userInput;

        auto [move, status] = board.processUserInput(userInput);

        if (!status || !board.validMoveWithCheck(move)) {
            cout << "Please provide a valid move" << "\n";
            continue;
        }


        engine.processMove(move);

        Move computerMove = engine.findBestMove();

        engine.processMove(computerMove);
    }
}

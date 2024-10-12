#include "Game.hpp"

#include <cstddef>
#include <iostream>

#include "Constants.hpp"
#include "Position.hpp"


using namespace std;

void Game::runGame() {
    while (!currentBoard.gameIsOver()) {
        currentBoard.displayBoard();

        string userInput;

        cin >> userInput;

        for (int i = 0; i < boardSize; ++i) {
            cout << "\n";
        }

        auto [move, status] = currentBoard.processUserInput(userInput);

        cout << move << endl;

        if (!status || !currentBoard.validMoveWithCheck(move)) {
            cout << "Please provide a valid move" << endl;
            continue;
        }
        currentBoard.processMove(move);
    }
}

#include "Game.hpp"

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

        auto [move, status] = processUserInput(userInput);

        cout << move << endl;

        if (!status || !currentBoard.validMoveWithCheck(move)) {
            cout << "Please provide a valid move" << endl;
            continue;
        }
        currentBoard.processMove(move);
    }
}

pair<Move, bool> Game::processUserInput(string& userInput) {
    if (userInput.size() != 4) {
        // not valid Move
        return { {}, false };
    }

    auto rowStart = static_cast<short>(userInput[1] - '0' - 1);
    auto columnStart = static_cast<short>(userInput[0] - 'a');
    auto rowEnd = static_cast<short>(userInput[3] - '0' - 1);
    auto columnEnd = static_cast<short>(userInput[2] - 'a');

    if (!validPosition(rowStart, columnStart) || !validPosition(rowEnd, columnEnd)) {
        return { {}, false };
    }

    return { Move(rowStart, columnStart, rowEnd, columnEnd), true };
}

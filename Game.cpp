#include "Game.hpp"

#include <iostream>

#include "Position.hpp"


using namespace std;

void Game::runGame() {
    while (!currentBoard.gameIsOver()) {
        string userInput;

        cin >> userInput;

        auto [move, status] = processUserInput(userInput);

        if (!status || !currentBoard.validMoveWithCheck(move)) {
            cout << "Please provide a valid move" << endl;
            continue;
        }
    }
}

pair<Move, bool> Game::processUserInput(string& userInput) {
    if (userInput.size() != 4) {
        // not valid Move
        return { {}, false };
    }

    auto rowStart = static_cast<short>(userInput[1] - 1);
    auto columnStart = static_cast<short>(userInput[0] - 'A');
    auto rowEnd = static_cast<short>(userInput[3] - 1);
    auto columnEnd = static_cast<short>(userInput[2] - 'A');

    if (validPosition(rowStart, columnStart)) {
        return { {}, false };
    }
    if (validPosition(rowEnd, columnEnd)) {
        return { {}, false };
    }

    return { Move(rowStart, columnStart, rowEnd, columnEnd), true };
}

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

        for (int i = 0; i < boardSize; ++i) {
            cout << "\n";
        }

        auto [move, status] = board.processUserInput(userInput);

        cout << move << endl;

        if (!status || !board.validMoveWithCheck(move)) {
            cout << "Please provide a valid move" << endl;
            continue;
        }


        engine.processMove(move);

        Move computerMove = engine.findBestMove();

        engine.processMove(computerMove);
    }
}

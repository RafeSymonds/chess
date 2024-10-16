#include "Game.hpp"

#include <cstddef>
#include <iostream>
#include <ostream>

#include "Constants.hpp"


using namespace std;

void Game::runGame() {
    Board& board = engine.getBoard();

    while (true) {
        board.displayBoard();

        string userInput;

        cin >> userInput;

        if (userInput == "quit") {
            return;
        }

        auto [move, status] = board.processUserInput(userInput);


        if (!status || !board.moveIsValidWithCheck(move, board.isWhiteTurn())) {
            cout << "Please provide a valid move" << "\n";
            continue;
        }

        cout << "Your move: " << move << "\n";

        engine.processMove(move);

        Move computerMove = engine.findBestMove();

        if (board.isGameOver()) {
            cout << "Game is over. You lost\n";
            return;
        }

        engine.processMove(computerMove);
    }
}

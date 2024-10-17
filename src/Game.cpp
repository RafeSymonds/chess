#include "Game.hpp"

#include <cstddef>
#include <iostream>

#include "Constants.hpp"


using namespace std;

void Game::runGame() {
    Board& board = engine.getBoard();

    while (true) {
        board.displayBoard();
        cout << board.getPawnAttacks(!board.isWhiteTurn()) << endl;

        string userInput;

        cin >> userInput;

        if (userInput == "quit") {
            return;
        }

        auto [move, status] = board.processUserInput(userInput);


        if (!status || !board.moveIsValidWithCheck(move, board.isWhiteTurn())) {
            cout << "Please provide a valid move"
                 << "\n";
            continue;
        }

        cout << "Your move: " << move << "\n";

        engine.processMove(move);

        Move computerMove = engine.findBestMove();

        engine.processMove(computerMove);
    }
}

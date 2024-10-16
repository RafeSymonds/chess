#include <cstdint>
#include <iostream>
#include <string>

#include <getopt.h>

#include <bits/getopt_core.h>
#include <bits/getopt_ext.h>

#include "Board.hpp"
#include "Constants.hpp"
#include "Game.hpp"
#include "Move.hpp"

using namespace std;

struct Options {
    string startBoard = defaultBoardPosition;
    int threadNum = 8;
    int depth = 6;
};

void printHelp(char* argv[]) {
    cout << "Usage: " << argv[0] << "";
}

void getMode(int argc, char* argv[], Options& options) {
    int choice = 0;
    int index = 0;

    option long_options[] = {
        {  "help",       no_argument, nullptr, 'h'},
        {"thread", required_argument, nullptr, 't'},
        { "depth", required_argument, nullptr, 'd'},
        { "start", required_argument, nullptr, 's'}
    };
    while ((choice = getopt_long(argc, argv, "ht:d:s:", long_options, &index)) != -1) {
        switch (choice) {
        case 'h': {
            printHelp(argv);
            exit(0);
            break;
        }
        case 't': {
            int arg = stoi(optarg);
            options.threadNum = arg;
            break;
        }
        case 'd': {
            int arg = stoi(optarg);
            options.depth = arg;
            break;
        }
        case 's': {
            string arg { optarg };
            options.startBoard = arg;
            break;
        }
        default: {
        }
        }
    }
}

array<uint64_t, numBoardSquares> generateKnightMoves() {
    std::array<uint64_t, numBoardSquares> moves {};
    for (int i = 0; i < numBoardSquares; ++i) {
        uint64_t move = 0;

        // Check if moving left 2 is possible
        if (i % boardSize >= 2) {
            if (i / boardSize >= 1) {
                move |= (1ULL << (i - boardSize - 2));   // left 2 up 1
            }
            if (i / boardSize <= boardSize - 2) {
                move |= (1ULL << (i + boardSize - 2));   // left 2 down 1
            }
        }

        // Check if moving left 1 is possible
        if (i % boardSize >= 1) {
            if (i / boardSize >= 2) {
                move |= (1ULL << (i - 2 * boardSize - 1));   // left 1 up 2
            }
            if (i / boardSize < boardSize - 2) {
                move |= (1ULL << (i + 2 * boardSize - 1));   // left 1 down 2
            }
        }


        // Check if moving right 2 is possible
        if (i % boardSize < boardSize - 2) {
            if (i / boardSize >= 1) {
                move |= (1ULL << (i - boardSize + 2));   // right 2 up 1
            }
            if (i / boardSize <= boardSize - 2) {
                move |= (1ULL << (i + boardSize + 2));   // right 2 down 1
            }
        }

        // Check if moving right 1 is possible
        if (i % boardSize < boardSize - 1) {
            if (i / boardSize >= 2) {
                move |= (1ULL << (i - 2 * boardSize + 1));   // right 1 up 2
            }
            if (i / boardSize < boardSize - 2) {
                move |= (1ULL << (i + 2 * boardSize + 1));   // right 1 down 2
            }
        }


        moves.at(i) = move;
    }
    return moves;
};

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin >> std::boolalpha;
    cout << std::boolalpha;

    Options options;
    getMode(argc, argv, options);

    auto knightMoves = generateKnightMoves();

    Game game(options.threadNum, options.startBoard, options.depth, &knightMoves);
    game.runGame();
}

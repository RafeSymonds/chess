#include <cstdint>
#include <iostream>
#include <string>

#include <getopt.h>

#include <bits/getopt_core.h>
#include <bits/getopt_ext.h>

#include "Board.hpp"
#include "Constants.hpp"
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
        {   "help",       no_argument, nullptr, 'h' },
        { "thread", required_argument, nullptr, 't' },
        {  "depth", required_argument, nullptr, 'd' },
        {  "start", required_argument, nullptr, 's' }
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
int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin >> std::boolalpha;
    cout << std::boolalpha;

    Options options;
    getMode(argc, argv, options);

    Board board("rnbqkbnr/pppppppp/8/8/3PP3/P1P2P1P/1P4P1/RNBQKBNR");

    uint64_t x = board.getWhitePawnAttacks();

    cout << "Start\n";
    for (int i = 0; i < 64; ++i) {
        if (i % 8 == 0 && i != 0) {
            cout << "\n";
        }
        if ((x & 1) == 1) {
            cout << "x ";
        } else {
            cout << "  ";
        }

        x >>= 1;
    }
    cout << "\n";


    cout << "Done" << endl;
}

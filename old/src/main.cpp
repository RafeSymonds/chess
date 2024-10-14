#include <string>

#include <getopt.h>

#include <bits/getopt_core.h>
#include <bits/getopt_ext.h>

#include "Constants.hpp"
#include "Game.hpp"

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

    Game game(options.threadNum, options.startBoard, options.depth);
    game.runGame();
}

#include <string>

#include "Constants.hpp"
#include "Game.hpp"

using namespace std;

int main() {
    // string testString = "4k2B/3p3p/8/1p6/4P3/P2P1P2/6PP/4K1R1";

    Game game(8, defaultBoardPosition, 5);
    game.runGame();
}

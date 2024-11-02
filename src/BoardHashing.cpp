#include "BoardHashing.hpp"

#include <cstddef>
#include <iostream>
#include <random>

#include "Constants.hpp"

BoardHashing::BoardHashing() {
    std::random_device rd;
    std::mt19937_64 gen(rd());

    // Define the distribution range
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
    for (size_t i = 0; i < pieceRandomNumbers.size(); ++i) {
        for (int j = 0; j < numBoardSquares; ++j) {
            pieceRandomNumbers[i][j] = dist(gen);
        }
    }

    turnRandomNumber[0] = dist(gen);
    turnRandomNumber[1] = dist(gen);
}

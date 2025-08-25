#ifndef GAMEBOARD_h
#define GAMEBOARD_h

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "Constants.hpp"

class Board {
private:
    std::array<uint64_t, numBitBoards> pieceBB {};

    // Bitboards for white attacks
    uint64_t whiteSlidingAttacking = 0;
    uint64_t whiteNonSlidingAttacking = 0;

    // Bitboards for black attacks
    uint64_t blackNonSlidingAttacking = 0;
    uint64_t blackSlidingAttacking = 0;


public:
};

#endif

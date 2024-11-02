#include <Constants.hpp>
#include <array>
#include <cstdint>

class BoardHashing {
public:
    std::array<std::array<uint64_t, numBoardSquares>, 12> pieceRandomNumbers;

    std::array<uint64_t, 2> turnRandomNumber;


    BoardHashing();
};

#include <climits>
#include <cstddef>
#include <utility>

#include "Board.hpp"
#include "Move.hpp"

class Worker {
private:
    Board board;

    int alpha;
    int beta;

public:
    Worker(const Board& board)
        : board(board)
        , alpha(INT_MIN)
        , beta(INT_MAX) {}

    std::pair<double, size_t> generateBestMove(int depth, const Move& move);
    void processMove(const Move& move);
    std::pair<double, size_t> alphaBetaPruning(const Move& move, int depth, double alpha, double beta);

    void setBoard(const Board& newBoard);
};

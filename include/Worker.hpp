#include <climits>

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

    double generateBestMove(int depth, const Move& move);
    void processMove(const Move& move);
    double alphaBetaPruning(const Move& move, int depth, double alpha, double beta);
};

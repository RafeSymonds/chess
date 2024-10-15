#include <climits>
#include <cstddef>

#include "Board.hpp"
#include "Move.hpp"

struct WorkerResult {
    double eval;
    double alpha;
    double beta;
    size_t positionsEvaluated;
    WorkerResult(double eval, double alpha, double beta, size_t positionsEvaluated)
        : eval(eval)
        , alpha(alpha)
        , beta(beta)
        , positionsEvaluated(positionsEvaluated) {}
};

class Worker {
private:
    Board board;

    size_t totalEvaluations {};

public:
    Worker(const Board& board)
        : board(board) {}

    WorkerResult generateBestMove(int depth, const Move& move, double alpha, double beta);
    void processMove(const Move& move);
    double alphaBetaPruning(const Move& move, int depth, double alpha, double beta);

    void setBoard(const Board& newBoard);

    void resetTotalEvaluations() { totalEvaluations = 0; }
};

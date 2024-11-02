#include <climits>
#include <cstddef>
#include <unordered_map>

#include "Board.hpp"
#include "Move.hpp"

struct WorkerResult {
    double eval;
    double alpha;
    double beta;
    size_t positionsEvaluated;
    size_t samePositionCount;
    WorkerResult(double eval, double alpha, double beta, size_t positionsEvaluated, size_t samePositionCount)
        : eval(eval)
        , alpha(alpha)
        , beta(beta)
        , positionsEvaluated(positionsEvaluated)
        , samePositionCount(samePositionCount) {}
};

class Worker {
private:
    Board board;

    std::unordered_map<uint64_t, double> boardHashes;

    size_t totalEvaluations {};
    size_t totalSamePositionsFound {};

public:
    Worker(const Board& board)
        : board(board) {}

    WorkerResult generateBestMove(int depth, const Move& move, double alpha, double beta);
    void processMove(const Move& move);
    double alphaBetaPruning(const Move& move, int depth, double alpha, double beta);

    void setBoard(const Board& newBoard);

    void resetData() {
        totalEvaluations = 0;
        totalSamePositionsFound = 0;
        boardHashes.clear();
    }
};

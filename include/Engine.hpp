
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <set>
#include <thread>
#include <vector>

#include "Board.hpp"
#include "Move.hpp"
#include "Worker.hpp"

struct MoveProcessing {
    Move move;
    int depth;
    double eval;

    MoveProcessing() = default;
    MoveProcessing(const Move& move, int depth, double eval)
        : move(move)
        , depth(depth)
        , eval(eval) {}
};

struct ProcessingFunctor {
    virtual bool operator()(const MoveProcessing& mp1, const MoveProcessing& mp2) const { return mp1.eval > mp2.eval; };
};

struct WhiteProccessingOrderFunctor : public ProcessingFunctor {
    bool operator()(const MoveProcessing& mp1, const MoveProcessing& mp2) const override {
        if (mp1.depth == mp2.depth) {
            return mp1.eval < mp2.eval;
        }
        return mp1.depth > mp2.depth;
    }
};

struct BlackProcessingOrderFunctor : public ProcessingFunctor {
    bool operator()(const MoveProcessing& mp1, const MoveProcessing& mp2) const override {
        if (mp1.depth == mp2.depth) {
            return mp1.eval > mp2.eval;
        }
        return mp1.depth > mp2.depth;
    }
};

struct WhiteBestMoveFunctor : public ProcessingFunctor {
    bool operator()(const MoveProcessing& mp1, const MoveProcessing& mp2) const override {
        if (mp1.depth != mp2.depth) {
            return mp1.depth < mp2.depth;
        }
        return mp1.eval > mp2.eval;
    }
};

struct BlackBestMoveFunctor : public ProcessingFunctor {
    bool operator()(const MoveProcessing& mp1, const MoveProcessing& mp2) const override {
        if (mp1.depth != mp2.depth) {
            return mp1.depth < mp2.depth;
        }
        return mp1.eval > mp2.eval;
    }
};

class Engine {
private:
    Board board;

    std::vector<Worker> workers;
    std::vector<std::thread> threads;

    std::mutex moveMutex;

    std::vector<Move> moves;

    std::priority_queue<MoveProcessing, std::vector<MoveProcessing>,
                        std::function<bool(const MoveProcessing&, const MoveProcessing&)>>
      movesNeedingProcessing;

    std::set<MoveProcessing, std::function<bool(const MoveProcessing&, const MoveProcessing&)>> finalMoveResults;

    std::condition_variable condition;
    std::condition_variable doneCondition;


    std::atomic<size_t> activeThreads { 0 };

    bool stop;

    int depth;

    double alpha;
    double beta;

    std::vector<std::pair<double, double>> alphaBetaValues;

    std::atomic<size_t> totalPositionsEvaluated;


    WhiteProccessingOrderFunctor whitePQFunctor;
    BlackProcessingOrderFunctor blackPQFunctor;


    WhiteBestMoveFunctor whiteSetFunctor;
    BlackBestMoveFunctor blackSetFunctor;


public:
    Engine(std::array<uint64_t, numBoardSquares>* knightMoves, BoardHashing& boardHashing);
    Engine(int threadNum, const Board& board, int depth);
    ~Engine();

    Board& getBoard();

    void generateWorkers();

    void workerTask(size_t index);

    Move findBestMove();

    void processMove(const Move& move);
};

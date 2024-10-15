
#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <thread>
#include <vector>

#include "Board.hpp"
#include "Move.hpp"
#include "Worker.hpp"


class Engine {
private:
    Board board;

    std::vector<Worker> workers;
    std::vector<std::thread> threads;
    std::vector<double> evaluation;

    std::mutex moveMutex;

    std::vector<Move> moves;

    std::condition_variable condition;
    std::condition_variable doneCondition;

    std::atomic<size_t> activeThreads { 0 };

    bool stop;

    int depth;

    double alpha;
    double beta;

    std::atomic<size_t> totalPositionsEvaluated;

public:
    Engine(std::array<uint64_t, numBoardSquares>& knightMoves);
    Engine(int threadNum, const Board& board, int depth);
    ~Engine();
    Board& getBoard();

    void generateWorkers();

    void workerTask(size_t index);

    Move findBestMove();

    void processMove(const Move& move);
};

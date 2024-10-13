#include "Engine.hpp"

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <thread>

#include "Board.hpp"
#include "Constants.hpp"


using namespace std;
Engine::Engine()
    : board(defaultBoardPosition)
    , workers(1, Worker(board))
    , threads(1)
    , moves(0)
    , stop(false)
    , depth(1) {
    generateWorkers();
}

Engine::Engine(int threadNum, const Board& board, int depth)
    : board(board)
    , workers(threadNum, Worker(board))
    , threads(threadNum)
    , stop(false)
    , depth(depth) {
    generateWorkers();
}

Engine::~Engine() {
    stop = true;
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
}

Board& Engine::getBoard() {
    return board;
}

void Engine::generateWorkers() {
    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i] = thread(&Engine::workerTask, this, i);
    }
}

Move Engine::findBestMove() {
    moves = board.getAllPossibleMoves();

    cout << "Moves len = " << moves.size() << endl;

    evaluation.resize(moves.size());


    condition.notify_all();

    {
        std::unique_lock<std::mutex> lock(moveMutex);
        doneCondition.wait(lock, [this] { return moves.empty(); });
    }

    if (board.isWhiteTurn()) {
        auto it = max_element(evaluation.begin(), evaluation.end());

        return moves[distance(evaluation.begin(), it)];
    }

    auto it = min(evaluation.begin(), evaluation.end());

    return moves[distance(evaluation.begin(), it)];
}

void Engine::workerTask(size_t index) {
    while (true) {
        Move move {};
        size_t moveIndex = -1;
        {
            std::unique_lock<std::mutex> lock(moveMutex);
            condition.wait(lock, [this] { return stop || !moves.empty(); });

            if (stop) {
                return;
            }

            move = moves.back();
            moves.pop_back();
            moveIndex = moves.size();
        }
        double eval = workers[index].generateBestMove(depth, move);

        {
            std::unique_lock<std::mutex> lock(moveMutex);
            evaluation[moveIndex] = eval;

            if (moves.empty()) {
                doneCondition.notify_one();
            }
        }
    }
}

void Engine::processMove(const Move& move) {
    board.processMove(move);
    for (Worker& worker : workers) {
        worker.processMove(move);
    }
}

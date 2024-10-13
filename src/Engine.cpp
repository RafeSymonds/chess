#include "Engine.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iterator>
#include <thread>

#include "Board.hpp"
#include "Constants.hpp"
#include "Move.hpp"


using namespace std;
Engine::Engine()
    : board(defaultBoardPosition)
    , workers(1, Worker(board))
    , threads(1)
    , moves(0)
    , stop(false)
    , depth(1)
    , totalPositionsEvaluated(0) {
    generateWorkers();
}

Engine::Engine(int threadNum, const Board& board, int depth)
    : board(board)
    , workers(threadNum, Worker(board))
    , threads(threadNum)
    , stop(false)
    , depth(depth)
    , totalPositionsEvaluated(0) {
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
    locale loc("");
    cout.imbue(loc);
    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i] = thread(&Engine::workerTask, this, i);
    }
}

Move Engine::findBestMove() {
    auto startTime = chrono::system_clock::now();

    moves = board.getAllPossibleMoves();
    vector<Move> savedMoves = moves;

    totalPositionsEvaluated = moves.size();

    cout << "Moves len = " << moves.size() << endl;

    evaluation.resize(moves.size());

    activeThreads = min(threads.size(), moves.size());

    cout << "Active threads " << activeThreads << endl;

    condition.notify_all();

    {
        std::unique_lock<std::mutex> lock(moveMutex);
        doneCondition.wait(lock, [this] { return moves.empty() && activeThreads == 0; });
    }

    auto it = evaluation.begin();

    if (board.isWhiteTurn()) {
        it = max_element(evaluation.begin(), evaluation.end());
    } else {
        it = min_element(evaluation.begin(), evaluation.end());
    }

    auto endTime = chrono::system_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    long long totalMicroseconds = duration.count();
    long long seconds = totalMicroseconds / 1000000;
    long long microseconds = totalMicroseconds % 1000000;

    cout << "Evaluated " << totalPositionsEvaluated << " positions in " << seconds << " seconds and " << microseconds
         << "\n";


    return savedMoves[distance(evaluation.begin(), it)];
}

void Engine::workerTask(size_t index) {
    size_t threadTotal = 0;
    while (true) {
        Move move {};
        size_t moveIndex = -1;
        {
            std::unique_lock<std::mutex> lock(moveMutex);
            condition.wait(lock, [this] { return stop || !moves.empty(); });

            if (stop) {
                return;
            }

            if (moves.empty()) {
                continue;
            }

            move = moves.back();
            moves.pop_back();
            moveIndex = moves.size();
        }
        auto [eval, positionEvaluated] = workers[index].generateBestMove(depth, move);

        cout << "New positionEvaluated=" << positionEvaluated << endl;

        {
            std::unique_lock<std::mutex> lock(moveMutex);
            evaluation[moveIndex] = eval;
            threadTotal += positionEvaluated;

            if (moves.empty() || activeThreads < threads.size()) {
                cout << "Thread " << index << " ended with a total of " << threadTotal << " evaluations" << endl;
                totalPositionsEvaluated += threadTotal;
                threadTotal += 0;

                if (--activeThreads == 0) {
                    doneCondition.notify_all();
                }
            }
        }
    }
}

void Engine::processMove(const Move& move) {
    board.processMove(move);
    for (Worker& worker : workers) {
        worker.setBoard(board);
    }
}

#include "Engine.hpp"

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iterator>
#include <limits>
#include <queue>
#include <thread>
#include <vector>

#include "Board.hpp"
#include "Constants.hpp"
#include "Move.hpp"


using namespace std;
Engine::Engine(std::array<uint64_t, numBoardSquares>* knightMoves)
    : board(defaultBoardPosition, knightMoves)
    , workers(1, Worker(board))
    , threads(1)
    , moves(0)
    , stop(false)
    , depth(1)
    , alpha(-numeric_limits<double>::max())
    , beta(numeric_limits<double>::max())
    , alphaBetaValues(1, { -numeric_limits<double>::max(), numeric_limits<double>::max() })
    , totalPositionsEvaluated(0) {
    generateWorkers();
}

Engine::Engine(int threadNum, const Board& board, int depth)
    : board(board)
    , workers(threadNum, Worker(board))
    , threads(threadNum)
    , stop(false)
    , depth(depth)
    , alpha(-numeric_limits<double>::max())
    , beta(numeric_limits<double>::max())
    , alphaBetaValues(depth, { -numeric_limits<double>::max(), numeric_limits<double>::max() })
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

    if (board.isWhiteTurn()) {
        movesNeedingProcessing = std::priority_queue<MoveProcessing, std::vector<MoveProcessing>,
                                                     std::function<bool(const MoveProcessing&, const MoveProcessing&)>>(
          [this](const MoveProcessing& mp1, const MoveProcessing& mp2) { return whitePQFunctor(mp1, mp2); });

        finalMoveResults = std::set<MoveProcessing, std::function<bool(const MoveProcessing&, const MoveProcessing&)>>(
          [this](const MoveProcessing& mp1, const MoveProcessing& mp2) { return whiteSetFunctor(mp1, mp2); });
    } else {
        cout << "Evaluting for black" << endl;

        movesNeedingProcessing = std::priority_queue<MoveProcessing, std::vector<MoveProcessing>,
                                                     std::function<bool(const MoveProcessing&, const MoveProcessing&)>>(
          [this](const MoveProcessing& mp1, const MoveProcessing& mp2) { return blackPQFunctor(mp1, mp2); });

        finalMoveResults = std::set<MoveProcessing, std::function<bool(const MoveProcessing&, const MoveProcessing&)>>(
          [this](const MoveProcessing& mp1, const MoveProcessing& mp2) { return blackSetFunctor(mp1, mp2); });
    }

    moves = board.getValidMovesWithCheck();

    totalPositionsEvaluated = moves.size();

    activeThreads = min(threads.size(), moves.size());

    cout << "Moves len=" << moves.size() << " Active threads=" << activeThreads << " Depth=" << depth << endl;

    condition.notify_all();

    {
        std::unique_lock<std::mutex> lock(moveMutex);
        doneCondition.wait(lock,
                           [this] { return moves.empty() && activeThreads == 0 && movesNeedingProcessing.empty(); });
    }

    if (finalMoveResults.empty()) {
        board.setGameOver();
        return {};
    }

    auto it = finalMoveResults.begin();
    if (board.isWhiteTurn()) {
        it = max_element(finalMoveResults.begin(), finalMoveResults.end(), whiteSetFunctor);
    } else {
        it = max_element(finalMoveResults.begin(), finalMoveResults.end(), blackSetFunctor);
    }

    auto endTime = chrono::system_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);

    long long totalMicroseconds = duration.count();
    long long seconds = totalMicroseconds / 1000000;
    long long milliseconds = totalMicroseconds / 1000;
    long long microseconds = totalMicroseconds % 1000000;

    cout << "Evaluated " << totalPositionsEvaluated << " positions in " << seconds << " seconds and " << milliseconds
         << " milliseconds and " << microseconds << " microseconds\n";


    if (finalMoveResults.empty()) {
        board.setGameOver();
        return {};
    }

    cout << "\nEvaluation: " << it->eval << "\n";

    return it->move;
}

void Engine::workerTask(size_t index) {
    size_t threadTotal = 0;
    while (true) {
        MoveProcessing moveProcessing {};

        Move move {};
        int currentDepth = 1;
        {
            std::unique_lock<std::mutex> lock(moveMutex);
            condition.wait(lock, [this] { return stop || !moves.empty() || !movesNeedingProcessing.empty(); });

            if (stop) {
                return;
            }

            if (moves.empty()) {
                moveProcessing = movesNeedingProcessing.top();
                movesNeedingProcessing.pop();

                move = moveProcessing.move;
                currentDepth = moveProcessing.depth;

            } else {
                move = moves.back();
                moves.pop_back();
                moveProcessing.move = move;
                moveProcessing.depth = currentDepth;
            }
        }

        workers[index].resetTotalEvaluations();
        WorkerResult workerResult = workers[index].generateBestMove(
          currentDepth - 1, move, alphaBetaValues[currentDepth - 1].first, alphaBetaValues[currentDepth - 1].second);

        {
            std::unique_lock<std::mutex> lock(moveMutex);

            cout << "Finisehd " << move << " with an eval=" << workerResult.eval << " at depth " << currentDepth
                 << " with positions evaluated=" << workerResult.positionsEvaluated << "\n"
                 << flush;

            moveProcessing.eval = workerResult.eval;

            finalMoveResults.insert(moveProcessing);

            if (board.isWhiteTurn()) {
                alphaBetaValues[currentDepth - 1].first
                  = max(alphaBetaValues[currentDepth - 1].first, workerResult.alpha);
            } else {
                alphaBetaValues[currentDepth - 1].second
                  = min(alphaBetaValues[currentDepth - 1].second, workerResult.beta);
            }


            if (currentDepth != depth) {
                movesNeedingProcessing.emplace(move, currentDepth + 1, workerResult.eval);
            }

            threadTotal += workerResult.positionsEvaluated;

            if ((moves.empty() && movesNeedingProcessing.empty())) {
                cout << "Thread " << index << " ended with a total of " << threadTotal << " evaluations" << endl;
                totalPositionsEvaluated += threadTotal;

                threadTotal = 0;

                if (--activeThreads == 0) {
                    doneCondition.notify_all();
                }
            }
        }
    }
}

void Engine::processMove(const Move& move) {
    board.processMoveWithReEvaulation(move);
    for (Worker& worker : workers) {
        worker.setBoard(board);
    }
}

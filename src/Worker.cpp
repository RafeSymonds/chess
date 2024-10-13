#include "Worker.hpp"

#include <cstddef>
#include <limits>

#include "Constants.hpp"
#include "Move.hpp"


using namespace std;


pair<double, size_t> Worker::generateBestMove(int depth, const Move& move) {
    PieceTypes pieceRemoved = board.processMove(move);
    vector<Move> moves = board.getAllPossibleMoves();


    size_t totalPositions = 0;

    bool whiteTurn = board.isWhiteTurn();
    double best = whiteTurn ? -numeric_limits<double>::max() : numeric_limits<double>::max();

    for (const Move& newMove : moves) {
        auto result
          = alphaBetaPruning(newMove, depth - 1, -numeric_limits<double>::max(), numeric_limits<double>::max());
        totalPositions += result.second;
        if (whiteTurn) {
            best = max(best, result.first);
        } else {
            best = min(best, result.first);
        }
    }

    board.unProcessMove(move, pieceRemoved);
    return { best, totalPositions };
}

pair<double, size_t> Worker::alphaBetaPruning(const Move& move, int depth, double alpha, double beta) {
    PieceTypes pieceRemoved = board.processMove(move);

    // cout << "Depth " << depth << "\n";

    if (depth == 0) {
        board.unProcessMove(move, pieceRemoved);
        return { 0, 0 };
    }

    double value = 0;
    vector<Move> moves = board.getAllPossibleMoves();


    pair<double, size_t> result = { 0, 0 };

    if (board.isWhiteTurn()) {
        value = -numeric_limits<double>::max();

        for (const Move& newMove : moves) {
            auto result = alphaBetaPruning(newMove, depth - 1, alpha, beta);

            value = max(value, result.first);

            if (value > beta) {
                break;
            }

            alpha = max(alpha, value);
        }
    } else {
        value = numeric_limits<double>::max();

        for (const Move& newMove : moves) {
            auto result = alphaBetaPruning(newMove, depth - 1, alpha, beta);
            value = min(value, result.first);

            if (value < alpha) {
                break;
            }

            beta = min(beta, value);
        }
    }

    board.unProcessMove(move, pieceRemoved);
    return { value, moves.size() + result.second };
}

void Worker::processMove(const Move& move) {
    board.processMove(move);
}

void Worker::setBoard(const Board& newBoard) {
    board = newBoard;
}
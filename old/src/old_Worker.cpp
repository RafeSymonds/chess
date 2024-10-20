#include "Worker.hpp"

#include <cstddef>
#include <limits>

#include "Constants.hpp"
#include "Move.hpp"


using namespace std;


WorkerResult Worker::generateBestMove(int depth, const Move& move, double alpha, double beta) {
    auto startEndPieces = board.processMove(move);
    vector<Move> moves = board.getAllPossibleMoves();


    if (moves.size() == 0) {
        board.unProcessMove(move, startEndPieces);
        return { board.isWhiteTurn() ? -numeric_limits<double>::max() : numeric_limits<double>::max(), alpha, beta, 1 };
    }


    double value = 0;

    if (board.isWhiteTurn()) {
        value = -numeric_limits<double>::max();

        for (const Move& newMove : moves) {
            double eval = alphaBetaPruning(newMove, depth - 1, alpha, beta);

            value = max(value, eval);

            if (value >= beta) {
                break;
            }

            alpha = max(alpha, value);
        }
    } else {
        value = numeric_limits<double>::max();

        for (const Move& newMove : moves) {
            double eval = alphaBetaPruning(newMove, depth - 1, alpha, beta);

            value = min(value, eval);
            if (value <= alpha) {
                break;
            }

            beta = min(beta, value);
        }
    }

    board.unProcessMove(move, startEndPieces);
    return { value, alpha, beta, moves.size() + totalEvaluations };
}

double Worker::alphaBetaPruning(const Move& move, int depth, double alpha, double beta) {
    auto startEndPieces = board.processMove(move);

    if (depth <= 0 && startEndPieces.second == none) {
        double eval = board.evaluation();
        board.unProcessMove(move, startEndPieces);

        ++totalEvaluations;

        return eval;
    }

    double value = 0;
    vector<Move> moves = board.getAllPossibleMoves();

    if (moves.size() == 0) {
        board.unProcessMove(move, startEndPieces);

        ++totalEvaluations;

        return board.isWhiteTurn() ? -numeric_limits<double>::max() : numeric_limits<double>::max();
    }


    if (board.isWhiteTurn()) {
        value = -numeric_limits<double>::max();

        for (const Move& newMove : moves) {
            double eval = alphaBetaPruning(newMove, depth - 1, alpha, beta);

            value = max(value, eval);

            if (value >= beta) {
                break;
            }

            alpha = max(alpha, value);
        }
    } else {
        value = numeric_limits<double>::max();

        for (const Move& newMove : moves) {
            double eval = alphaBetaPruning(newMove, depth - 1, alpha, beta);

            value = min(value, eval);

            if (value <= alpha) {
                break;
            }

            beta = min(beta, value);
        }
    }

    board.unProcessMove(move, startEndPieces);
    return value;
}

void Worker::processMove(const Move& move) {
    board.processMove(move);
}

void Worker::setBoard(const Board& newBoard) {
    board = newBoard;
}

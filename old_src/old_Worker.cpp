#include "Worker.hpp"

#include <cstddef>
#include <limits>

#include "Constants.hpp"
#include "Move.hpp"


using namespace std;


WorkerResult Worker::generateBestMove(int depth, const Move& move, double alpha, double beta) {
    int startEndPieces = board.processMoveWithReEvaulation(move);

    vector<Move> moves = board.getValidMovesWithCheck();


    if (moves.size() == 0) {
        double eval = board.isWhiteTurn() ? -numeric_limits<double>::max() : numeric_limits<double>::max();
        board.unProcessMove(move, startEndPieces);

        if (board.isWhiteTurn() && eval < beta) {
            alpha = max(alpha, eval);
        } else if (!board.isWhiteTurn() && eval > alpha) {
            beta = min(beta, eval);
        }

        return { eval, alpha, beta, 1 };
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

    board.unProcessMoveWithReEvaulation(move, startEndPieces);
    return { value, alpha, beta, moves.size() + totalEvaluations };
}

double Worker::alphaBetaPruning(const Move& move, int depth, double alpha, double beta) {
    int previousValue = board.processMoveWithReEvaulation(move);

    if (depth <= 0 && previousValue == -1) {
        double eval = board.evaluation();
        board.unProcessMoveWithReEvaulation(move, previousValue);

        ++totalEvaluations;

        return eval;
    }

    double value = 0;

    vector<Move> moves = board.getValidMovesWithCheck();

    if (moves.empty()) {
        double eval = board.isWhiteTurn() ? -numeric_limits<double>::max() : numeric_limits<double>::max();
        board.unProcessMoveWithReEvaulation(move, previousValue);

        ++totalEvaluations;

        return eval;
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

    board.unProcessMoveWithReEvaulation(move, previousValue);
    return value;
}

void Worker::processMove(const Move& move) {
    board.processMoveWithReEvaulation(move);
}

void Worker::setBoard(const Board& newBoard) {
    board = newBoard;
}

#include "Worker.hpp"

#include <iostream>
#include <limits>

#include "Constants.hpp"
#include "Move.hpp"


using namespace std;

double Worker::generateBestMove(int depth, const Move& move) {
    return alphaBetaPruning(move, depth - 1, numeric_limits<double>::min(), numeric_limits<double>::max());
}

double Worker::alphaBetaPruning(const Move& move, int depth, double alpha, double beta) {
    PieceTypes pieceRemoved = board.processMove(move);


    if (depth == 0) {
        return 5;
    }

    double value = 0;
    vector<Move> moves = board.getAllPossibleMoves();

    if (board.isWhiteTurn()) {
        value = numeric_limits<double>::min();

        for (const Move& move : moves) {
            value = max(value, alphaBetaPruning(move, depth - 1, alpha, beta));

            if (value > beta) {
                break;
            }

            alpha = max(alpha, value);
        }
    } else {
        value = numeric_limits<double>::max();

        for (const Move& move : moves) {
            value = max(value, alphaBetaPruning(move, depth - 1, alpha, beta));

            if (value < beta) {
                break;
            }

            beta = min(beta, value);
        }
    }

    board.unProcessMove(move, pieceRemoved);
    return value;
}

void Worker::processMove(const Move& move) {
    board.processMove(move);
}

#include "Worker.hpp"

#include <cstddef>
#include <limits>

#include "Constants.hpp"
#include "Move.hpp"


using namespace std;


pair<double, size_t> Worker::generateBestMove(int depth, const Move& move) {
    PieceTypes pieceRemoved = board.processMove(move);
    vector<Move> moves = board.getAllPossibleMoves();


    if (moves.size() == 0) {
        board.unProcessMove(move, pieceRemoved);
        return { board.isWhiteTurn() ? -numeric_limits<double>::max() : numeric_limits<double>::max(), 1 };
    }


    size_t totalPositions = moves.size();

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

    if (depth <= 0 && pieceRemoved == none) {
        double eval = board.evaluation();
        board.unProcessMove(move, pieceRemoved);
        return { eval, 1 };
    }

    double value = 0;
    vector<Move> moves = board.getAllPossibleMoves();

    if (moves.size() == 0) {
        board.unProcessMove(move, pieceRemoved);
        return { board.isWhiteTurn() ? -numeric_limits<double>::max() : numeric_limits<double>::max(), 1 };
    }

    size_t totalPositionsEvaluated = 0;

    if (board.isWhiteTurn()) {
        value = -numeric_limits<double>::max();

        for (const Move& newMove : moves) {
            auto result = alphaBetaPruning(newMove, depth - 1, alpha, beta);

            totalPositionsEvaluated += result.second;

            value = max(value, result.first);

            if (value >= beta) {
                break;
            }

            alpha = max(alpha, value);
        }
    } else {
        value = numeric_limits<double>::max();

        for (const Move& newMove : moves) {
            auto result = alphaBetaPruning(newMove, depth - 1, alpha, beta);

            totalPositionsEvaluated += result.second;

            value = min(value, result.first);

            if (value <= alpha) {
                break;
            }

            beta = min(beta, value);
        }
    }

    board.unProcessMove(move, pieceRemoved);
    return { value, totalPositionsEvaluated };
}

void Worker::processMove(const Move& move) {
    board.processMove(move);
}

void Worker::setBoard(const Board& newBoard) {
    board = newBoard;
}
#include "Board.hpp"

#include <cstddef>
#include <iostream>

#include "Constants.hpp"
#include "Position.hpp"

using namespace std;

Board::Board(const string& fen) {
    size_t lastPos = 0;
    size_t pos = 0;
    int row = 0;

    while (pos != string::npos) {
        pos = fen.find('/', lastPos);

        string line = fen.substr(lastPos, pos - lastPos);

        int column = 0;

        cout << line << endl;

        for (char c : line) {
            switch (c) {
            case 'p':
                board[row][column] = PieceTypes::whitePawn;
                break;
            case 'P':
                board[row][column] = PieceTypes::blackPawn;
                break;
            case 'n':
                board[row][column] = PieceTypes::whiteKnight;
                break;
            case 'N':
                board[row][column] = PieceTypes::blackKnight;
                break;
            case 'b':
                board[row][column] = PieceTypes::whiteBishop;
                break;
            case 'B':
                board[row][column] = PieceTypes::blackBishop;
                break;
            case 'r':
                board[row][column] = PieceTypes::whiteRook;
                break;
            case 'R':
                board[row][column] = PieceTypes::blackRook;
                break;
            case 'q':
                board[row][column] = PieceTypes::whiteQueen;
                break;
            case 'Q':
                board[row][column] = PieceTypes::blackQueen;
                break;
            case 'k':
                board[row][column] = PieceTypes::whiteKing;
                whiteKingPosition = { row, column };
                break;
            case 'K':
                board[row][column] = PieceTypes::blackKing;
                blackKingPosition = { row, column };
                break;
            default:
                int val = c - '0';
                column += val;
            }
            ++column;
        }
        if (pos == string::npos) {
            return;
        }
        ++row;
        column = 0;
        size_t diff = pos - lastPos;
        lastPos = pos + 1;
        pos += diff;
    }
}

PieceTypes Board::getPieceType(const Position& position) const {
    return board[position.row][position.column];
}
PieceTypes& Board::getPieceType(const Position& position) {
    return board[position.row][position.column];
}


bool Board::knightAttacks(const Position& position) const {
    PieceTypes knightColor = getPieceType(position) > none ? whiteKnight : blackKnight;

    for (const Position& pos : knightMoves) {
        Position newPos = position + pos;
        if (validPosition(newPos) && getPieceType(newPos) == knightColor) {
            return true;
        }
    }
    return false;
}

bool Board::straightAttacks(const Position& position) const {
    PieceTypes target = getPieceType(position);
    PieceTypes queenColor = target > none ? whiteQueen : blackQueen;
    PieceTypes rookColor = target > none ? whiteRook : blackRook;

    for (const Position& direction : straightDirections) {
        for (int step = 1; step < boardSize; ++step) {
            Position newPos = position + Position(direction.row * step, direction.column * step);

            if (!validPosition(newPos)) {
                break;
            }

            PieceTypes piece = getPieceType(newPos);

            if (sameSide(target, piece)) {
                break;
            }

            if (piece == queenColor || piece == rookColor) {
                return true;
            }
        }
    }
    return false;
}

bool Board::diagonalAttacks(const Position& position) const {
    PieceTypes target = getPieceType(position);
    PieceTypes queenColor = target > none ? whiteQueen : blackQueen;
    PieceTypes bishopColor = target > none ? whiteBishop : blackBishop;

    for (const Position& direction : diagonalDirections) {
        for (int step = 1; step < boardSize; ++step) {
            Position newPos = position + Position(direction.row * step, direction.column * step);

            if (!validPosition(newPos)) {
                break;
            }

            PieceTypes piece = getPieceType(newPos);

            if (sameSide(target, piece)) {
                break;
            }

            if (piece == queenColor || piece == bishopColor) {
                return true;
            }
        }
    }
    return false;
}

bool Board::kingInCheck(const Position& position) const {
    return knightAttacks(position) || straightAttacks(position) || diagonalAttacks(position);
}

bool isValidMoveForPawn(const Move& move, PieceTypes pieceType) {
    int direction = (pieceType == whitePawn) ? 1 : -1;
    if (move.end.column == move.start.column) {
        if (move.end.row == move.start.row + direction) return true;
        if ((pieceType == whitePawn && move.start.row == 1 && move.end.row == move.start.row + 2)
            || (pieceType == blackPawn && move.start.row == 6 && move.end.row == move.start.row - 2)) {
            return true;
        }
    } else if (std::abs(move.end.column - move.start.column) == 1) {
        return move.end.row == move.start.row + direction;
    }
    return false;
}

bool isValidMoveForKnight(const Move& move) {
    int rowDiff = std::abs(move.end.row - move.start.row);
    int colDiff = std::abs(move.end.column - move.start.column);
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

bool isValidMoveForRook(const Move& move) {
    return (move.start.row == move.end.row || move.start.column == move.end.column);
}

bool isValidMoveForBishop(const Move& move) {
    return std::abs(move.end.row - move.start.row) == std::abs(move.end.column - move.start.column);
}

bool isValidMoveForQueen(const Move& move) {
    return isValidMoveForRook(move) || isValidMoveForBishop(move);
}

bool isValidMoveForKing(const Move& move) {
    int rowDiff = std::abs(move.end.row - move.start.row);
    int colDiff = std::abs(move.end.column - move.start.column);
    return (rowDiff <= 1 && colDiff <= 1);
}

bool Board::validMoveNoCheck(const Move& move) const {
    if (move.start == move.end || sameSide(getPieceType(move.end), getPieceType(move.start))) {
        return false;
    }
    PieceTypes piece = getPieceType(move.start);

    switch (piece) {
    case whitePawn:
    case blackPawn:
        return isValidMoveForPawn(move, piece);
    case whiteKnight:
    case blackKnight:
        return isValidMoveForKnight(move);
    case whiteBishop:
    case blackBishop:
        return isValidMoveForBishop(move);
    case whiteRook:
    case blackRook:
        return isValidMoveForRook(move);
    case whiteQueen:
    case blackQueen:
        return isValidMoveForQueen(move);
    case whiteKing:
    case blackKing:
        return isValidMoveForKing(move);
    default:
        return false;
    }
}

bool Board::validMoveWithCheck(const Move& move) {
    if (!validMoveNoCheck(move)) {
        return false;
    }

    PieceTypes& startPlace = getPieceType(move.start);
    PieceTypes& endPlace = getPieceType(move.end);
    PieceTypes targetPiece = getPieceType(move.end);

    endPlace = startPlace;
    startPlace = none;

    bool valid = endPlace > none ? !kingInCheck(whiteKingPosition) : !kingInCheck(blackKingPosition);

    startPlace = endPlace;
    endPlace = targetPiece;

    return valid;
}

bool Board::gameIsOver() const {
    return gameOver;
}

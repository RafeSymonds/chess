#include "Board.hpp"

#include <cstddef>
#include <iostream>
#include <new>

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
            PieceTypes piece = getPieceTypeFromChar(c);

            board[row][column] = piece;

            if (piece == PieceTypes::whiteKing) {
                whiteKingPosition = { row, column };
            } else if (piece == PieceTypes::blackKing) {
                blackKingPosition = { row, column };
            } else if (piece == PieceTypes::none) {
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
    PieceTypes knightColor = getPieceType(position) < none ? whiteKnight : blackKnight;

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
    PieceTypes queenColor = target < none ? whiteQueen : blackQueen;
    PieceTypes rookColor = target < none ? whiteRook : blackRook;

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
    PieceTypes queenColor = target < none ? whiteQueen : blackQueen;
    PieceTypes bishopColor = target < none ? whiteBishop : blackBishop;

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

bool Board::isValidMoveForPawn(const Move& move, PieceTypes pieceType) const {
    int direction = (pieceType == whitePawn) ? -1 : 1;
    if (move.end.column == move.start.column) {
        if (move.end.row == move.start.row + direction) return true;
        if ((pieceType == whitePawn && move.start.row == 6 && move.end.row == move.start.row - 2)
            || (pieceType == blackPawn && move.start.row == 1 && move.end.row == move.start.row + 2)) {
            return true;
        }
    } else if (std::abs(move.end.column - move.start.column) == 1) {
        return move.end.row == move.start.row + direction;
    }
    return false;
}

bool Board::isValidMoveForKnight(const Move& move) const {
    int rowDiff = std::abs(move.end.row - move.start.row);
    int colDiff = std::abs(move.end.column - move.start.column);
    return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2);
}

bool Board::isValidMoveForRook(const Move& move) const {
    if (move.start.row != move.end.row && move.start.column != move.end.column) {
        return false;
    }

    if (move.start.row == move.end.row) {
        int startCol = min(move.start.column, move.end.column) + 1;
        int endCol = max(move.start.column, move.end.column);

        for (; startCol < endCol; ++startCol) {
            if (getPieceType(move.start + Position(0, startCol)) != none) {
                return false;
            }
        }

    } else {
        int startRow = min(move.start.row, move.end.row) + 1;
        int endRow = max(move.start.row, move.end.row);

        for (; startRow < endRow; ++startRow) {
            if (getPieceType(move.start + Position(startRow, 0)) != none) {
                return false;
            }
        }
    }

    return true;
}

bool Board::isValidMoveForBishop(const Move& move) const {
    if (std::abs(move.start.row - move.end.row) != std::abs(move.start.column - move.end.column)) {
        return false;
    }

    int rowDirection = (move.end.row > move.start.row) ? 1 : -1;
    int colDirection = (move.end.column > move.start.column) ? 1 : -1;

    int row = move.start.row + rowDirection;
    int col = move.start.column + colDirection;

    while (row != move.end.row && col != move.end.column) {
        if (getPieceType(Position(row, col)) != none) {
            return false;
        }
        row += rowDirection;
        col += colDirection;
    }

    return true;
}

bool Board::isValidMoveForQueen(const Move& move) const {
    return isValidMoveForRook(move) || isValidMoveForBishop(move);
}

bool Board::isValidMoveForKing(const Move& move) const {
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

void Board::processMove(const Move& move) {
    PieceTypes& startPlace = getPieceType(move.start);
    PieceTypes& endPlace = getPieceType(move.end);

    endPlace = startPlace;
    startPlace = none;
}

bool Board::gameIsOver() const {
    return gameOver;
}

void Board::displayBoard() const {
    int rowNum = 1;
    for (const auto& row : board) {
        cout << rowNum << " ";
        for (const auto& piece : row) {
            cout << getCharFromPieceType(piece) << " ";
        }
        rowNum += 1;
        cout << "\n";
    }
    char column = 'a';


    cout << "  ";

    for (int i = 0; i < boardSize; ++i) {
        cout << char(column + i) << " ";
    }
    cout << "\n";

    cout << flush;
}

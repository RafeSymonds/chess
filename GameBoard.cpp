#include "GameBoard.h"

#include <cstddef>
#include <cstdlib>

#include "Constants.h"
#include "Move.h"

using namespace std;

GameBoard::GameBoard(const string& fen) {
    size_t lastPos = 0;
    size_t pos = 0;
    short row = 0;

    while ((pos = fen.find('/')) != string::npos) {
        string line = fen.substr(lastPos, lastPos - pos);

        short column = 0;

        for (char c : line) {
            switch (c) {
            case 'p':
                board[row][column] = Pieces::whitePawn;
                break;
            case 'P':
                board[row][column] = Pieces::blackPawn;
                break;
            case 'n':
                board[row][column] = Pieces::whiteKnight;
                break;
            case 'N':
                board[row][column] = Pieces::blackKnight;
                break;
            case 'b':
                board[row][column] = Pieces::whiteBishop;
                break;
            case 'B':
                board[row][column] = Pieces::blackBishop;
                break;
            case 'r':
                board[row][column] = Pieces::whiteRook;
                break;
            case 'R':
                board[row][column] = Pieces::blackRook;
                break;
            case 'q':
                board[row][column] = Pieces::whiteQueen;
                break;
            case 'Q':
                board[row][column] = Pieces::blackQueen;
                break;
            case 'k':
                board[row][column] = Pieces::whiteKing;
                whiteKingPosition = { row, column };
                break;
            case 'K':
                board[row][column] = Pieces::blackKing;
                blackKingPosition = { row, column };
                break;
            }
            ++column;
        }
        ++row;
    }
}

vector<Move> GameBoard::generateMoves() const {
    vector<Move> moves;
    if (whiteTurn) {
        for (int r = 0; r < BOARD_SIZE; ++r) {
            for (int c = 0; c < BOARD_SIZE; ++c) {
                if (board[r][c] > 0) {
                    // process move
                }
            }
        }
    }

    return moves;
}

bool GameBoard::gameIsOver() const {
    return gameOver;
}

bool GameBoard::validPosition(int row, int column) {
    return row >= 0 && row < 8 && column >= 0 && column < 8;
}

bool GameBoard::kingInCheck(Move& move) {
    return false;
}

Pieces& GameBoard::getPieceFromPosition(Position& position) {
    return board[position.row][position.column];
}


bool GameBoard::bishopMoveCheck(Move& move) {
    short rowDiff = abs(move.start.row - move.end.row);
    short colDiff = abs(move.start.row - move.end.row);

    if (rowDiff != colDiff) {
        return false;
    }

    short row = min(move.start.row, move.end.row);
    short endRow = max(move.start.row, move.end.row);

    short col = min(move.start.column, move.end.column);

    for (; row < move.end.row; ++row, ++col) {
        if (board[row][col] != Pieces::none) {
            return false;
        }
    }
    return true;
}


bool GameBoard::rookMoveCheck(Move& move) {
    short rowDiff = abs(move.start.row - move.end.row);
    short colDiff = abs(move.start.row - move.end.row);

    if (rowDiff != 0 && colDiff != 0) {
        return false;
    }


    short row = min(move.start.row, move.end.row);
    short maxRow = min(move.start.row, move.end.row);

    for (; row < maxRow; ++row) {
        if (board[row][move.start.column] != Pieces::none) {
            return false;
        }
    }

    short col = min(move.start.column, move.end.column);
    short maxCol = min(move.start.column, move.end.column);

    for (; col < maxCol; ++col) {
        if (board[move.start.row][col] != Pieces::none) {
            return false;
        }
    }

    return true;
}
bool GameBoard::validMove(Move& move) {
    // check peice can move in that direction
    if (move.start == move.end) {
        return false;
    }


    Pieces& piece = getPieceFromPosition(move.start);
    Pieces& pieceAtTarget = getPieceFromPosition(move.end);


    if ((pieceAtTarget > Pieces::none && piece > Pieces::none)
        || (pieceAtTarget < Pieces::none && piece < Pieces::none)) {
        return false;
    }


    switch (piece) {
    case Pieces::whitePawn:

        if (move.end.row >= move.start.row) {
            return false;
        }
        if (move.start.row - move.end.row == 2 && (move.start.row != 6 || move.start.column != move.end.column)) {
            return false;
        }
        if (abs(move.start.column - move.end.column) > 1) {
            return false;
        }

        break;
    case Pieces::blackPawn:
        if (move.end.row <= move.start.row) {
            return false;
        }
        if (move.start.row - move.end.row == 2 && (move.start.row != 1 || move.start.column != move.end.column)) {
            return false;
        }
        if (abs(move.start.column - move.end.column) > 1) {
            return false;
        }
        break;
    case Pieces::whiteKnight:
    case Pieces::blackKnight: {
        short rowDiff = abs(move.start.row - move.end.row);
        short colDiff = abs(move.start.row - move.end.row);

        if (rowDiff != 2 && colDiff != 2) {
            return false;
        }
        if (rowDiff != 1 && colDiff != 1) {
            return false;
        }
        if (rowDiff == colDiff) {
            return false;
        }
    } break;
    case Pieces::whiteBishop:
    case Pieces::blackBishop:
        if (!bishopMoveCheck(move)) {
            return false;
        }
        break;
    case Pieces::whiteRook:
    case Pieces::blackRook:
        if (!rookMoveCheck(move)) {
            return false;
        }

        break;
    case Pieces::whiteQueen:
    case Pieces::blackQueen:
        if (!bishopMoveCheck(move) && !rookMoveCheck(move)) {
            return false;
        }
        break;
    case Pieces::whiteKing:
    case Pieces::blackKing:
        if (abs(move.start.row - move.end.row) > 1) {
            return false;
        }
        if (abs(move.start.row - move.end.row) > 1) {
            return false;
        }
        if (move.start.row == move.end.row && move.start.column == move.end.column) {
            return false;
        }
        break;
    case Pieces::none:
        cout << "Tried to move a piece that does not exist." << endl;
        exit(1);
        break;
    }

    // check if our king is in check in the new position
    return kingInCheck(move);
}

void GameBoard::processMove(Move& move) {}

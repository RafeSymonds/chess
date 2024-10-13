#include "Board.hpp"

#include <algorithm>
#include <cctype>
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

bool Board::pawnAttacks(const Position& position) const {
    PieceTypes pawnColor = getPieceType(position) < none ? whitePawn : blackPawn;

    int direction = pawnColor == whitePawn ? 1 : -1;

    for (int dc = -1; dc < 2; dc += 2) {
        Position newPos = position + Position(direction, dc);
        if (validPosition(newPos) && getPieceType(newPos) == pawnColor) {
            return true;
        }
    }

    return false;
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
    return pawnAttacks(position) || knightAttacks(position) || straightAttacks(position) || diagonalAttacks(position);
}

bool Board::isValidMoveForPawn(const Move& move, PieceTypes pieceType) const {
    int direction = (pieceType == whitePawn) ? -1 : 1;
    if (move.end.column == move.start.column) {
        if (getPieceType(move.end) != none) {
            return false;
        }
        if (move.end.row == move.start.row + direction) {
            return true;
        }
        if ((pieceType == whitePawn && move.start.row == 6 && move.end.row == move.start.row - 2)
            || (pieceType == blackPawn && move.start.row == 1 && move.end.row == move.start.row + 2)) {
            return true;
        }
        return false;
    }
    return move.end.row == move.start.row + direction && abs(move.start.column - move.end.column) == 1
        && getPieceType(move.end) != none;
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
            if (getPieceType(Position(move.start.row, startCol)) != none) {
                return false;
            }
        }

    } else {
        int startRow = min(move.start.row, move.end.row) + 1;
        int endRow = max(move.start.row, move.end.row);

        for (; startRow < endRow; ++startRow) {
            if (getPieceType(Position(startRow, move.start.column)) != none) {
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
    PieceTypes piece = getPieceType(move.start);

    if ((piece > none && !whiteTurn) || (piece < none && whiteTurn)) {
        return false;
    }

    if (move.start == move.end || sameSide(piece, getPieceType(move.end))) {
        return false;
    }

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

    if (endPlace == whiteKing) {
        whiteKingPosition = move.end;
    } else if (endPlace == blackKing) {
        blackKingPosition = move.end;
    }

    bool valid = endPlace > none ? !kingInCheck(whiteKingPosition) : !kingInCheck(blackKingPosition);

    startPlace = endPlace;
    endPlace = targetPiece;


    return valid;
}

PieceTypes Board::processMove(const Move& move) {
    PieceTypes& startPlace = getPieceType(move.start);
    PieceTypes& endPlace = getPieceType(move.end);

    PieceTypes originalEndPlace = endPlace;

    endPlace = startPlace;
    startPlace = none;

    if (endPlace == whiteKing) {
        whiteKingPosition = move.end;
    } else if (endPlace == blackKing) {
        blackKingPosition = move.end;
    }

    whiteTurn = !whiteTurn;

    return originalEndPlace;
}

void Board::unProcessMove(const Move& move, PieceTypes pieceRemoved) {
    PieceTypes& startPlace = getPieceType(move.start);
    PieceTypes& endPlace = getPieceType(move.end);

    if (endPlace == whiteKing) {
        whiteKingPosition = move.start;
    } else if (endPlace == blackKing) {
        blackKingPosition = move.start;
    }


    startPlace = endPlace;
    endPlace = pieceRemoved;

    whiteTurn = !whiteTurn;
}

bool Board::gameIsOver() const {
    return gameOver;
}

bool Board::isWhiteTurn() const {
    return whiteTurn;
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

pair<Move, bool> Board::processUserInput(const string& userInput) const {
    size_t numChars = userInput.size();

    int rowEnd = userInput[numChars - 1] - '1';
    int colEnd = userInput[numChars - 2] - 'a';

    if (userInput[0] >= 'a' && userInput[0] <= 'h') {
        // pawn move
        int directionFrom = whiteTurn ? 1 : -1;

        if (numChars == 2) {
            if (getPieceType(Position(rowEnd + directionFrom, colEnd)) == none) {
                directionFrom *= 2;
            }

            return { Move(rowEnd + directionFrom, colEnd, rowEnd, colEnd), true };
        }

        int columnStart = userInput[0] - 'a';
        return { Move(rowEnd + directionFrom, columnStart, rowEnd, colEnd), true };
    }

    int rowStart = -1;
    int colStart = -1;


    for (size_t i = 1; i < userInput.size() - 2; ++i) {
        if (userInput[i] == 'x') {
            continue;
        }
        if (isdigit(userInput[i]) != 0) {
            rowStart = userInput[i] - '1';
        } else {
            colStart = userInput[i] - 'a';
        }
    }

    Position endPosition(rowEnd, colEnd);

    if (userInput[0] == 'N') {
        // knight
        for (const Position& pos : knightMoves) {
            Position newPos = endPosition + pos;
            if ((rowStart != -1 && newPos.row != rowStart) && (colStart != -1 && newPos.column != colStart)) {
                continue;
            }

            if (!validPosition(newPos)) {
                continue;
            }
            PieceTypes piece = getPieceType(newPos);
            if ((whiteTurn && piece == whiteKnight) || (!whiteTurn && piece == blackKnight)) {
                return { Move(newPos.row, newPos.column, rowEnd, colEnd), true };
            }
        }
    }
    if (userInput[0] == 'R' || userInput[0] == 'Q') {
        bool isRook = (userInput[0] == 'R');

        for (const Position& direction : straightDirections) {
            for (int step = 1; step < boardSize; ++step) {
                Position newPos = endPosition + direction * step;
                if (!validPosition(newPos)) {
                    break;
                }
                if ((rowStart != -1 && newPos.row != rowStart) && (colStart != -1 && newPos.column != colStart)) {
                    continue;
                }

                PieceTypes piece = getPieceType(newPos);

                if (piece == none) {
                    continue;
                }

                if (isRook && ((whiteTurn && piece == whiteRook) || (!whiteTurn && piece == blackRook))) {
                    return { Move(newPos.row, newPos.column, rowEnd, colEnd), true };
                }

                if (!isRook && ((whiteTurn && piece == whiteQueen) || (!whiteTurn && piece == blackQueen))) {
                    return { Move(newPos.row, newPos.column, rowEnd, colEnd), true };
                }

                break;
            }
        }
    }
    if (userInput[0] == 'B' || userInput[0] == 'Q') {
        bool isBishop = (userInput[0] == 'B');

        for (const Position& direction : diagonalDirections) {
            for (int step = 1; step < boardSize; ++step) {
                Position newPos = endPosition + direction * step;
                if (!validPosition(newPos)) {
                    break;
                }
                if ((rowStart != -1 && newPos.row != rowStart) && (colStart != -1 && newPos.column != colStart)) {
                    continue;
                }

                PieceTypes piece = getPieceType(newPos);

                if (piece == none) {
                    continue;
                }

                if (isBishop && ((whiteTurn && piece == whiteBishop) || (!whiteTurn && piece == blackBishop))) {
                    return { Move(newPos.row, newPos.column, rowEnd, colEnd), true };
                }

                if (!isBishop && ((whiteTurn && piece == whiteQueen) || (!whiteTurn && piece == blackQueen))) {
                    return { Move(newPos.row, newPos.column, rowEnd, colEnd), true };
                }

                break;
            }
        }
    }
    return { Move(), true };
}

vector<Move> Board::getPawnMoves(const Position& position) const {
    std::vector<Move> moves;
    int direction = whiteTurn ? -1 : 1;
    int startRow = whiteTurn ? 6 : 1;

    Position newPos = position + Position(direction, 0);

    if (!validPosition(newPos)) {
        return moves;
    }

    if (getPieceType(newPos) == none) {
        moves.emplace_back(position.row, position.column, newPos.row, newPos.column);

        Position newPos = position + Position(direction * 2, 0);
        if (position.row == startRow && getPieceType(newPos) == none) {
            moves.emplace_back(position.row, position.column, newPos.row, newPos.column);
        }
    }

    for (int dc = -1; dc <= 1; dc += 2) {
        Position newPos = position + Position(direction, dc);
        if (!validPosition(newPos)) {
            continue;
        }

        PieceTypes target = getPieceType(newPos);

        if ((target < none && whiteTurn) || (target > none && !whiteTurn)) {
            moves.emplace_back(position.row, position.column, newPos.row, newPos.column);
        }
    }

    return moves;
}
vector<Move> Board::getKnightMoves(const Position& position) const {
    vector<Move> moves;

    for (const Position& pos : knightMoves) {
        Position newPos = position + pos;
        if (!validPosition(newPos)) {
            continue;
        }

        PieceTypes target = getPieceType(newPos);

        if (target == none || (target < none && whiteTurn) || (target > none && !whiteTurn)) {
            moves.emplace_back(position.row, position.column, newPos.row, newPos.column);
        }
    }
    return moves;
}
vector<Move> Board::getRookMoves(const Position& position) const {
    vector<Move> moves;

    PieceTypes piece = getPieceType(position);

    for (const Position& direction : straightDirections) {
        for (int step = 1; step < boardSize; ++step) {
            Position newPos = position + direction * step;
            if (!validPosition(newPos)) {
                break;
            }

            PieceTypes target = getPieceType(newPos);

            if (sameSide(piece, target)) {
                break;
            }

            if (target == none) {
                moves.emplace_back(position.row, position.column, newPos.row, newPos.column);
            } else {
                // hit enemy
                moves.emplace_back(position.row, position.column, newPos.row, newPos.column);
                break;
            }
        }
    }

    return moves;
}
vector<Move> Board::getBishopMoves(const Position& position) const {
    vector<Move> moves;

    PieceTypes piece = getPieceType(position);

    for (const Position& direction : diagonalDirections) {
        for (int step = 1; step < boardSize; ++step) {
            Position newPos = position + direction * step;
            if (!validPosition(newPos)) {
                break;
            }

            PieceTypes target = getPieceType(newPos);

            if (sameSide(piece, target)) {
                break;
            }

            if (target == none) {
                moves.emplace_back(position.row, position.column, newPos.row, newPos.column);
            } else {
                // hit enemy
                moves.emplace_back(position.row, position.column, newPos.row, newPos.column);
                break;
            }
        }
    }

    return moves;
}
vector<Move> Board::getQueenMoves(const Position& position) const {
    vector<Move> rookMoves = getRookMoves(position);
    vector<Move> bishopMoves = getBishopMoves(position);


    vector<Move> queenMoves;
    queenMoves.reserve(rookMoves.size() + bishopMoves.size());

    queenMoves.insert(queenMoves.end(), rookMoves.begin(), rookMoves.end());
    queenMoves.insert(queenMoves.end(), bishopMoves.begin(), bishopMoves.end());

    return queenMoves;
}
vector<Move> Board::getKingMoves(const Position& position) const {
    vector<Move> moves;

    PieceTypes piece = getPieceType(position);

    for (const Position& direction : kingMoves) {
        Position newPos = position + direction;

        if (!validPosition(newPos)) {
            continue;
        }

        PieceTypes target = getPieceType(newPos);

        if (!sameSide(piece, target)) {
            moves.emplace_back(position.row, position.column, newPos.row, newPos.column);
        }
    }

    return moves;
}


vector<Move> Board::getAllPossibleMoves() {
    vector<Move> allMoves;

    vector<Move> moves;

    for (int row = 0; row < boardSize; ++row) {
        for (int column = 0; column < boardSize; ++column) {
            Position position(row, column);
            PieceTypes pieceType = getPieceType(position);

            if ((whiteTurn && pieceType > none) || (!whiteTurn && pieceType < none)) {
                switch (pieceType) {
                case whitePawn: {
                case blackPawn:
                    moves = getPawnMoves(position);
                    break;
                }
                case whiteKnight:
                case blackKnight: {
                    moves = getKnightMoves(position);
                    break;
                }
                case whiteBishop:
                case blackBishop: {
                    moves = getBishopMoves(position);
                    break;
                }
                case whiteRook:
                case blackRook: {
                    moves = getRookMoves(position);
                    break;
                }
                case whiteQueen:
                case blackQueen: {
                    moves = getQueenMoves(position);
                    break;
                }
                case whiteKing:
                case blackKing: {
                    moves = getKingMoves(position);
                    break;
                }
                default:
                    break;
                }

                for (const Move& move : moves) {
                    if (validMoveWithCheck(move)) {
                        allMoves.push_back(move);
                    }
                }
            }
        }
    }

    return allMoves;
}

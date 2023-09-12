#include "pieces.h"
#include "board.h"

// Coord
bool Coord::operator==(const Coord &b) const {
    return (row == b.row) && (column == b.column);
}

// hash function for Coord
// gives every Coord a unique value from -1 to 63
std::size_t std::hash<Coord>::operator()(const Coord c) const {
        int hash_val = (c.row << 3) | c.column;
        return hash<int>()(hash_val);
}

// Piece
Piece::Piece(Color color) : color(color) {};

Piece::~Piece() = default;

bool Piece::coord_in_range(Coord coord) const {
    return (coord.row >= 0 && coord.row < 8 && coord.column >= 0 && coord.column < 8);
}
const Piece* Piece::get_piece_from_coord(const Piece* const board[8][8], Coord square) {
    return board[square.row][square.column];
}

std::unordered_set<Coord> Piece::possible_moves_helper(const Piece* const board[8][8], Coord current_square, bool cross_check, bool diagonal_check) const {
    std::unordered_set<Coord> possible_squares;
    for (int row_offsetter = -1; row_offsetter <= 1; row_offsetter++) {
        for (int column_offsetter = -1; column_offsetter <= 1; column_offsetter++) {
            if (((row_offsetter | column_offsetter) == 0) // if both offsets are equal to 0
            || (!cross_check && ((row_offsetter & column_offsetter) == 0)) // or if cross_check is disabled and one of the offsets is equal to 0
            || (!diagonal_check && ((row_offsetter & column_offsetter) != 0))) { // or if diagonal_check is disabled and neither of the offsets are equal to 0
                continue;
            }
            Coord check_square {current_square.row + row_offsetter, current_square.column + column_offsetter};
            while (coord_in_range(check_square)) {
                const Piece *check_piece = get_piece_from_coord(board, check_square);
                if (check_piece == nullptr) {
                    possible_squares.insert(check_square);
                }
                else if (check_piece->color != color) {
                    possible_squares.insert(check_square);
                    break;
                } else {
                    break;
                }
                check_square.row += row_offsetter;
                check_square.column += column_offsetter;
            }
        }
    }
    return possible_squares;

}

// King 
const char* King::get_symbol() const {
    return color ? "♚" : "♔";
}

const std::string King::get_piece_name() const {
    return "KING";
}

std::unordered_set<Coord> King::possible_moves(const Piece* const board[8][8], Coord current_square) const {
    std::unordered_set<Coord> possible_squares;
    for (int row_offset = -1; row_offset <= 1; row_offset++) {
        for (int column_offset = -1; column_offset <= 1; column_offset++) {
            if ((row_offset | column_offset) == 0) { // continue if the row offset and column offset are both 0
                continue;
            }
            Coord check_square {current_square.row + row_offset, current_square.column + column_offset};
            if (coord_in_range(check_square)) {
                const Piece *check_piece = get_piece_from_coord(board, check_square);
                if (check_piece == nullptr || check_piece->color != color) {
                    possible_squares.insert(check_square);
                }
            }
        }
    }
    return possible_squares;
}


// Queen
const char* Queen::get_symbol() const {
    return color ? "♛" : "♕";
}
const std::string Queen::get_piece_name() const {
    return "QUEEN";
}

std::unordered_set<Coord> Queen::possible_moves(const Piece* const board[8][8], Coord current_square) const {
    return possible_moves_helper(board, current_square, true, true);
}

// Knight 
const char* Knight::get_symbol() const {
    return color ? "♞" : "♘";
}
const std::string Knight::get_piece_name() const {
    return "KNIGHT";
}

std::unordered_set<Coord> Knight::possible_moves(const Piece* const board[8][8], Coord current_square) const {
    std::unordered_set<Coord> possible_squares;

    int move_list[] = {-2, -1, 1, 2};
    for (int row_offset : move_list) {
        for (int column_offset : move_list) {
            if ((row_offset + column_offset) % 2 != 0) { // ensures that there is a -1 or 1 being paired with a -2 or 2
                Coord check_square {current_square.row + row_offset, current_square.column + column_offset};
                if (coord_in_range(check_square)) {
                    const Piece* check_piece = get_piece_from_coord(board, check_square);
                    if (check_piece == nullptr || check_piece->color != color) {
                        possible_squares.insert(check_square);
                    }
                }
            }
        }
    }
    return possible_squares;
}

// Bishop
const char* Bishop::get_symbol() const {
    return color ? "♝" : "♗";
}
const std::string Bishop::get_piece_name() const {
    return "BISHOP";
}

std::unordered_set<Coord> Bishop::possible_moves(const Piece* const board[8][8], Coord current_square) const {
    return possible_moves_helper(board, current_square, false, true);
}


// Rook
const char* Rook::get_symbol() const {
    return color ? "♜" : "♖";
}
const std::string Rook::get_piece_name() const {
    return "ROOK";
}

std::unordered_set<Coord> Rook::possible_moves(const Piece* const board[8][8], Coord current_square) const {
    return possible_moves_helper(board, current_square, true, false);
}


// Pawn
const char* Pawn::get_symbol() const {
    return color ? "♟︎" : "♙";
}
const std::string Pawn::get_piece_name() const {
    return "PAWN";
}

std::unordered_set<Coord> Pawn::possible_moves(const Piece* const board[8][8], Coord current_square) const {
    std::unordered_set<Coord> possible_squares;

    int next_row = current_square.row + (color ? 1 : -1);
    Coord front_square {next_row, current_square.column};
    if (coord_in_range(front_square)) {
        const Piece* front_piece = get_piece_from_coord(board, front_square);
        if (front_piece == nullptr) {
            possible_squares.insert(front_square);
        }
    }
    if (color == WHITE && current_square.row == 6 || color == BLACK && current_square.row == 1) {
        Coord front2_square {next_row + (color ? 1 : -1), current_square.column};
        const Piece* front_piece = get_piece_from_coord(board, front_square);
        if (front_piece == nullptr) {
            possible_squares.insert(front2_square);
        }
    }

    Coord diag_squares[] = {
        Coord {next_row, current_square.column + 1},
        Coord {next_row, current_square.column - 1}
    };
    for (Coord diag_square : diag_squares) {
        if (coord_in_range(diag_square)) {
            const Piece* diag_piece = get_piece_from_coord(board, diag_square);
            if (diag_piece != nullptr && diag_piece->color != color) {
                possible_squares.insert(diag_square);
            }
        }
    }
    return possible_squares;
}

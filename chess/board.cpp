#include "board.h"


Coord Board::input_to_coord(std::string input) { // it's magic!
    char rowchar = input[1];
    char colchar = toupper(input[0]);

    const char first_row = '8';
    const char first_col = 'A';
    const char last_row = '1';
    const char last_col = 'H';

    if ((input.length() != 2) || 
    !(rowchar >= last_row && rowchar <= first_row) || 
    !(colchar >= first_col && colchar <= last_col)) {
        return NULLCOORD;
    }
    int row = first_row - rowchar;
    int column = colchar - first_col;
    return Coord {row, column};
}

void Board::display(Color side) const {
    display(side, NULLCOORD, {});
}

void Board::display(Color side, Coord selected_piece, std::unordered_set<Coord> possible_moves) const {
    std::system("clear");

    std::cout << C_BOLD;
    if (side == WHITE) {
        std::cout << "     A   B   C   D   E   F   G   H " << std::endl;
    } else if (side == BLACK) {
        std::cout << "     H   G   F   E   D   C   B   A " << std::endl;
    }

    int row, col;

    for (int i = 0; i < 8; i++) {
        row = side ? 7-i : i;
        std::cout << 8-row << "  ";
        for (int j = 0; j < 8; j++) {
            col = side ? 7-j : j;
            bool option = false;
            Coord square = Coord {row, col};
            Piece* p = get_piece(square);
            if (possible_moves.find(square) != possible_moves.end()) {
                option = true;
            }
            if (p == nullptr) {
                std::cout << "| ";
                if (option) {
                    std::cout << C_OPTION << "•";
                } else {
                std::cout << " ";
                }
            } else {
                std::cout << "| ";
                if (selected_piece != NULLCOORD && selected_piece == square) {
                    std::cout << C_SELECT;
                }
                else if (option) {
                    std::cout << C_OPTION;
                }
                else if (p->color == BLACK) {
                    std::cout << C_BLACK;
                } else {
                    std::cout << C_WHITE;
                }
                std::cout << p->get_symbol();
            }
            std::cout << " " << C_RESET << C_BOLD;
        }
        std::cout << std::endl << "---+---+---+---+---+---+---+---+---" << std::endl;
    }
    std::cout << C_RESET << std::endl;
}

bool Board::valid_square(Coord square) const {
    Piece* piece = get_piece(square);
    return (piece != nullptr && piece->color == turn);
}

std::string Board::get_square_name(Coord square) const {
    Piece* piece = get_piece(square);
    assert(piece != nullptr);
    return piece->get_piece_name();
}

bool Board::pawn_promoted(Coord end_square) const {
    Piece* piece = get_piece(end_square);
    // if a pawn has reached the end row
    return (piece != nullptr && piece->get_piece_name() == "PAWN" && end_square.row == (piece->color ? 7 : 0));
}
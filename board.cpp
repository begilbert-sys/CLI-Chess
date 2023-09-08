#include <iostream>
#include "board.h"

Board::Board() {
    turn = WHITE;
    
    board[0][0] = new Rook(BLACK);
    board[0][1] = new Knight(BLACK);
    board[0][2] = new Bishop(BLACK);
    board[0][3] = new Queen(BLACK);
    board[0][4] = new King(BLACK);
    board[0][5] = new Bishop(BLACK);
    board[0][6] = new Knight(BLACK);
    board[0][7] = new Rook(BLACK);

    for (int i = 0; i < 8; i++) {
        board[1][i] = new Pawn(BLACK);
        board[6][i] = new Pawn(WHITE);
    }
    board[7][0] = new Rook(WHITE);
    board[7][1] = new Knight(WHITE);
    board[7][2] = new Bishop(WHITE);
    board[7][3] = new Queen(WHITE);
    board[7][4] = new King(WHITE);
    board[7][5] = new Bishop(WHITE);
    board[7][6] = new Knight(WHITE);
    board[7][7] = new Rook(WHITE);

    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }
}

Board::~Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j];
        }
    }
}

Piece* Board::get_square(Coord coord) const {
    return board[coord.row][coord.column];
}

Coord Board::input_to_coord(std::string input) { // it's magic!
    char rowchar = input[1];
    char colchar = input[0];

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
    int column = toupper(colchar) - first_col;
    return Coord {row, column};
}

void Board::display(Coord selected_piece, std::unordered_set<Coord> possible_moves) const {
    std::system("clear");
    std::cout << C_BOLD;
    std::cout << "     A   B   C   D   E   F   G   H " << std::endl;

    for (int i = 0; i < 8; i++) {
        std::cout << 8-i << "  ";
        for (int j = 0; j < 8; j++) {
            bool option = false;
            Coord square = Coord {i, j};
            Piece* p = get_square(square);
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
                std::cout << p->get_piece();
            }
            std::cout << " " << C_RESET << C_BOLD;
        }
        std::cout << std::endl << "---+---+---+---+---+---+---+---+---" << std::endl;
    }
    std::cout << C_RESET << std::endl;
}

void Board::display() const {
    display(NULLCOORD, {});
}




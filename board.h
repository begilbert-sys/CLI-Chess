#ifndef BOARD
#define BOARD


#include "pieces.h"
#include "ansicolors.h"


class Board {
public:
    Board();
    ~Board();
    void display() const;
    void display(Coord selected_piece, std::unordered_set<Coord> possible_moves) const;
    std::unordered_set<Coord> get_possible_moves(Coord square);
    static Coord input_to_coord(std::string input);
    Piece* get_piece(Coord coord) const;
    void move_piece(Coord start_square, Coord end_square);

    Piece* board[8][8];
    bool turn;

private:
    bool white_castle;
    bool black_castle;
    std::unordered_set<Coord> get_possible_castles(Coord square);
    bool handle_castling(Coord start_square, Coord end_square);
    Coord white_en_passant;
    Coord black_en_passant;

};

#endif
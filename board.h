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
    bool get_possible_promo(Coord end_square);
    bool promote_pawn(Coord square, std::string promo_piece);

    Piece* board[8][8];
    bool turn;

private:
    bool white_castle;
    bool black_castle;
    std::unordered_set<Coord> get_possible_castles(Coord square);
    bool handle_castling(Coord start_square, Coord end_square);

    Coord white_en_passant;
    Coord black_en_passant;
    Coord get_possible_en_passant(Coord square);
    void handle_en_passant(Coord start_square, Coord end_square);


    std::unordered_set<Coord> get_possible_check_moves;

};

#endif
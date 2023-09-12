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
    static Coord input_to_coord(std::string input);

    std::unordered_set<Coord> get_possible_moves(Coord square) const;


    Piece* get_piece(Coord coord) const;
    void move_piece(Coord start_square, Coord end_square);

    bool pawn_promoted(Coord end_square) const; // returns true if a promotion is possible
    bool promote_pawn(Coord square, std::string promo_piece);
    
    bool king_in_check(Color color) const;


    Piece* board[8][8];
    Color turn;

private:
    bool white_castle;
    bool black_castle;
    void adjust_for_castles(std::unordered_set<Coord> &possible_moves, Coord square) const;
    bool handle_castling(Coord start_square, Coord end_square);

    Coord white_en_passant;
    Coord black_en_passant;
    void adjust_for_en_passant(std::unordered_set<Coord> &possible_moves, Coord square) const; // returns the coord of the en passant 
    void handle_en_passant(Coord start_square, Coord end_square);

    void adjust_possible_king_moves(std::unordered_set<Coord> &possible_moves, Coord square) const;



    std::unordered_set<Coord> get_piece_coords(Color color) const;
    Coord get_king(Color color) const;
    
};

#endif
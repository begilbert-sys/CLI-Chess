#ifndef BOARD_MANAGER
#define BOARD_MANAGER


#include "pieces.h"
#include <cassert>

class Board_Manager {
public:
    Board_Manager();
    ~Board_Manager();

    std::unordered_set<Coord> get_possible_moves(Coord square) const;
    void move_piece(Coord start_square, Coord end_square);
    bool promote_pawn(Coord square, std::string promo_piece);
    
    bool king_in_check(Color color) const;

    Piece* board[8][8];
    Color turn;

protected:
    Board_Manager(const Board_Manager &b); // private copy constructor
    Piece* get_piece(Coord coord) const;

    bool white_castle;
    bool black_castle;
    void adjust_for_castles(std::unordered_set<Coord> &possible_moves, Coord square) const;
    bool handle_castling(Coord start_square, Coord end_square);

    Coord white_en_passant;
    Coord black_en_passant;
    void adjust_for_en_passant(std::unordered_set<Coord> &possible_moves, Coord square) const; // returns the coord of the en passant 
    void handle_en_passant(Coord start_square, Coord end_square);

    void adjust_for_check(std::unordered_set<Coord> &possible_moves, Coord square) const;


    std::unordered_set<Coord> get_piece_coords(Color color) const;
    Coord get_king(Color color) const;
    
};

#endif
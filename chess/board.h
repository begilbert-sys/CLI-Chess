#ifndef BOARD
#define BOARD


#include "pieces.h"
#include "ansicolors.h"
#include "board_manager.h"
#include <iostream>
#include <cassert>


class Board : public Board_Manager {
public:
    using Board_Manager::Board_Manager;

    void display() const;
    void display(Coord selected_piece, std::unordered_set<Coord> possible_moves) const;
    static Coord input_to_coord(std::string input);

    bool valid_square(Coord square) const;
    std::string get_square_name(Coord square) const;

    bool pawn_promoted(Coord end_square) const; // returns true if a promotion is possible
};

#endif
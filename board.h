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
    Piece* get_square(Coord coord) const;

//private:
    Piece* board[8][8];
    bool turn;
};

#endif
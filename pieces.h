#ifndef PIECES
#define PIECES

#include <unordered_set>

class Board; // forward declaration 

const bool BLACK = 1;
const bool WHITE = 0;

// Coord
struct Coord {
    int row;
    int column;
    bool operator==(const Coord &c) const;
};

template<> struct std::hash<Coord> {
    std::size_t operator()(const Coord c) const;
};

const Coord NULLCOORD {-1, -1};

// Pieces

class Piece {
public:
    Piece(bool c);
    virtual ~Piece();
    virtual const char* get_symbol() const = 0;
    virtual const std::string get_piece_name() const = 0;
    virtual std::unordered_set<Coord> possible_moves(const Piece* const board[8][8], Coord current_square) const = 0;
    bool color;

protected:
    bool coord_in_range(Coord coord) const;
    const Piece* get_piece_from_coord(const Piece* const board[8][8], Coord square) const;
    std::unordered_set<Coord> possible_moves_helper(const Piece* const board[8][8], Coord current_square, bool cross_check, bool diagonal_check) const;
};


class King : public Piece {
public:
    using Piece::Piece;
    virtual const char* get_symbol() const;
    virtual const std::string get_piece_name() const;
    std::unordered_set<Coord> possible_moves(const Piece* const board[8][8], Coord current_square) const;
};


class Queen : public Piece {
public:
    using Piece::Piece;
    virtual const char* get_symbol() const;
    virtual const std::string get_piece_name() const;
    virtual std::unordered_set<Coord> possible_moves(const Piece* const board[8][8], Coord current_square) const;
};

class Knight : public Piece {
public:
    using Piece::Piece;
    virtual const char* get_symbol() const;
    virtual const std::string get_piece_name() const;
    virtual std::unordered_set<Coord> possible_moves(const Piece* const board[8][8], Coord current_square) const;
};

class Bishop : public Piece {
public:
    using Piece::Piece;
    virtual const char* get_symbol() const;
    virtual const std::string get_piece_name() const;
    virtual std::unordered_set<Coord> possible_moves(const Piece* const board[8][8], Coord current_square) const;
};

class Rook : public Piece {
public:
    using Piece::Piece;
    virtual const char* get_symbol() const;
    virtual const std::string get_piece_name() const;
    virtual std::unordered_set<Coord> possible_moves(const Piece* const board[8][8], Coord current_square) const;
};

class Pawn : public Piece {
public:
    using Piece::Piece;
    virtual const char* get_symbol() const;
    virtual const std::string get_piece_name() const;
    virtual std::unordered_set<Coord> possible_moves(const Piece* const board[8][8], Coord current_square) const;
};

#endif
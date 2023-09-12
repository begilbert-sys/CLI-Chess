#include "board_manager.h"

Board_Manager::Board_Manager() {
    turn = WHITE;
    // these variables track whether or not either player is allowed to castle
    white_castle = true;
    black_castle = true;

    white_en_passant = NULLCOORD;
    black_en_passant = NULLCOORD;
    
    board[0][0] = new Rook(BLACK);
    board[0][1] = new Knight(BLACK);
    board[0][2] = new Bishop(BLACK);
    board[0][3] = new Queen(BLACK);
    board[0][4] = new King(BLACK);
    board[0][5] = new Bishop(BLACK);
    board[0][6] = new Knight(BLACK);
    board[0][7] = new Rook(BLACK);

    for (int i = 0; i < 8; i++) {
        board[1][i] = nullptr;//new Pawn(BLACK);
        board[6][i] = nullptr;//new Pawn(WHITE);
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

Board_Manager::~Board_Manager() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            delete board[i][j];
        }
    }
}

Board_Manager::Board_Manager(const Board_Manager &b) {
    turn = b.turn;
    white_castle = b.white_castle;
    black_castle = b.black_castle;
    white_en_passant = b.white_en_passant;
    black_en_passant = b.black_en_passant;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = b.board[i][j];
            if (piece == nullptr) {
                board[i][j] = nullptr;
            }
            else if (piece->get_piece_name() == "KING") {
                board[i][j] = new King(piece->color);
            }
            else if (piece->get_piece_name() == "QUEEN") {
                board[i][j] = new Queen(piece->color);
            }
            else if (piece->get_piece_name() == "BISHOP") {
                board[i][j] = new Bishop(piece->color);
            }
            else if (piece->get_piece_name() == "KNIGHT") {
                board[i][j] = new Knight(piece->color);
            }
            else if (piece->get_piece_name() == "ROOK") {
                board[i][j] = new Rook(piece->color);
            }
            else if (piece->get_piece_name() == "PAWN") {
                board[i][j] = new Pawn(piece->color);
            }
        }
    }
}

std::unordered_set<Coord> Board_Manager::get_possible_moves(Coord square) const {
    Piece* piece = get_piece(square);
    std::unordered_set<Coord> possible_moves = piece->possible_moves(board, square);

    adjust_for_castles(possible_moves, square);

    adjust_for_en_passant(possible_moves, square);

    adjust_for_check(possible_moves, square);

    return possible_moves;
}

void Board_Manager::move_piece(Coord start_square, Coord end_square) {

    turn = turn ? WHITE : BLACK;
    Piece* start_piece = get_piece(start_square);
    Piece* end_piece = get_piece(end_square);
    handle_en_passant(start_square, end_square);

    
    bool castled = handle_castling(start_square, end_square);
    if (castled) {
        return;
    }

    board[start_square.row][start_square.column] = nullptr;

    if (end_piece != nullptr) {
        delete board[end_square.row][end_square.column];
    }

    board[end_square.row][end_square.column] = start_piece;
}

bool Board_Manager::promote_pawn(Coord square, std::string promo_piece) {
    Piece* piece = get_piece(square);
    Color color = piece->color;
    assert(piece->get_piece_name() == "PAWN");
    delete board[square.row][square.column];
    if (promo_piece == "QUEEN") {
        board[square.row][square.column] = new Queen(color);
    }
    else if (promo_piece == "BISHOP") {
        board[square.row][square.column] = new Bishop(color);
    }
    else if (promo_piece == "KNIGHT") {
        board[square.row][square.column] = new Knight(color);
    }
    else if (promo_piece == "ROOK") {
        board[square.row][square.column] = new Rook(color);
    }
    else {
        return false;
    }
    return true;
}

bool Board_Manager::king_in_check(Color color) const {
    Coord king_coord = get_king(color);
    std::unordered_set<Coord> enemy_pieces = get_piece_coords(color ? WHITE : BLACK);
    for (Coord coord : enemy_pieces) {
        Piece* enemy_piece = get_piece(coord);
        std::unordered_set<Coord> possible_enemy_moves = enemy_piece->possible_moves(board, coord);
        if (possible_enemy_moves.count(king_coord) == 1) {
            return true;
        }
    }
    return false;
}




Piece* Board_Manager::get_piece(Coord coord) const {
    return board[coord.row][coord.column];
}


void Board_Manager::adjust_for_castles(std::unordered_set<Coord> &possible_moves, Coord square) const {
    Piece* piece = get_piece(square);
    if (!(((piece->color == WHITE && white_castle) || (piece->color == BLACK && black_castle)) &&
    (piece->get_piece_name() == "ROOK" || piece->get_piece_name() == "KING")))  {
        return;
    }

    int row = piece->color ? 0 : 7; // row is 0 if BLACK, 7 if WHITE

    int left_side_clear = true;
    int right_side_clear = true;
    for (int i = 1; i <= 3; i++) {
        if (board[row][i] != nullptr) {
            left_side_clear = false;
        }
    }
    for (int i = 5; i <= 6; i++) {
        if (board[row][i] != nullptr) {
            right_side_clear = false;
        }
    }

    const Coord left_rook_square = Coord{row, 0};
    const Coord right_rook_square = Coord{row, 7};
    const Coord king_square = Coord{row, 4};

    if ((square == left_rook_square && left_side_clear) ||
        (square == right_rook_square && right_side_clear)) { 
        possible_moves.insert(king_square); // Add King's position
    }
    if (square == king_square && left_side_clear) {
        possible_moves.insert(left_rook_square);
    }
    if (square == king_square && right_side_clear) {
        possible_moves.insert(right_rook_square);
    }
}

bool Board_Manager::handle_castling(Coord start_square, Coord end_square) {
    Piece* start_piece = get_piece(start_square);
    Piece* end_piece = get_piece(end_square);

    // disable castling if the rook or king are moved
    if (start_piece->get_piece_name() == "ROOK" || start_piece->get_piece_name() == "KING") {
        if (white_castle && start_piece->color == WHITE) {
            white_castle = false;
        }
        else if (black_castle && start_piece->color == BLACK) {
            black_castle = false;
        }
    }

    if (end_piece == nullptr || start_piece->color != end_piece->color) { // if the colors of the pieces match, a castle is taking place
        return false;
    }
    int row = start_piece->color ? 0 : 7; // row is 0 if BLACK, 7 if WHITE
    const Coord left_rook_square = Coord{row, 0};
    const Coord right_rook_square = Coord{row, 7};
    const Coord king_square = Coord{row, 4};

    Piece* rook;
    Piece* king;
    if (start_square == left_rook_square || start_square == right_rook_square) {
        rook = start_piece;
        king = end_piece;
    } else {
        rook = end_piece;
        king = start_piece;
    }

    if ((start_square == left_rook_square && end_square == king_square) 
    || (start_square == king_square && end_square == left_rook_square)) {
        board[row][2] = king;
        board[row][3] = rook;
        board[row][0] = nullptr;
        board[row][4] = nullptr;
    } else {
        board[row][5] = rook;
        board[row][6] = king;
        board[row][4] = nullptr;
        board[row][7] = nullptr;
    }
    return true;
}

void Board_Manager::adjust_for_en_passant(std::unordered_set<Coord> &possible_moves, Coord square) const {
    Piece* piece = get_piece(square);
    if (black_en_passant != NULLCOORD && piece->color == WHITE) {
        Coord top_left = Coord{square.row - 1, square.column - 1};
        Coord top_right = Coord{square.row - 1, square.column + 1};
        if (top_left == black_en_passant) {
            possible_moves.insert(top_left);
        } else if (top_right == black_en_passant) {
            possible_moves.insert(top_right);
        }
    }
    else if (white_en_passant != NULLCOORD && piece->color == BLACK) {
        Coord top_left = Coord{square.row + 1, square.column - 1};
        Coord top_right = Coord{square.row + 1, square.column + 1};
        if (top_left == white_en_passant) {
            possible_moves.insert(top_left);
        } else if (top_right == white_en_passant) {
            possible_moves.insert(top_right);
        }
    }
}

void Board_Manager::handle_en_passant(Coord start_square, Coord end_square) {

    if (end_square == white_en_passant) {
        Piece* captured_pawn = board[end_square.row-1][end_square.column];
        assert(captured_pawn->get_piece_name() == "PAWN");
        delete board[end_square.row+1][end_square.column];
        board[end_square.row+1][end_square.column] = nullptr;
    } 
    else if (end_square == black_en_passant) {
        Piece* captured_pawn = board[end_square.row+1][end_square.column];
        assert(captured_pawn->get_piece_name() == "PAWN");
        delete board[end_square.row+1][end_square.column];
        board[end_square.row+1][end_square.column] = nullptr;
    }

    white_en_passant = NULLCOORD;
    black_en_passant = NULLCOORD;

    Piece* start_piece = get_piece(start_square);
    Piece* end_piece = get_piece(end_square);

    if (start_piece->get_piece_name() == "PAWN" && end_piece == nullptr) {
        if (start_piece->color == WHITE && (start_square.row - end_square.row == 2)) {
            white_en_passant = Coord {end_square.row+1, end_square.column};
        }
        else if (start_piece->color == BLACK && (end_square.row - start_square.row == 2)) {
            black_en_passant = Coord {end_square.row-1, end_square.column};
        }
    }
}

void Board_Manager::adjust_for_check(std::unordered_set<Coord> &possible_moves, Coord square) const {
    std::unordered_set<Coord> potential_moves = possible_moves;
    for (Coord c : potential_moves) {
        Board_Manager test_board_manager(*this);
        test_board_manager.move_piece(square, c);
        if (test_board_manager.king_in_check(turn)) {
            possible_moves.erase(c);
        }
    }
}


std::unordered_set<Coord> Board_Manager::get_piece_coords(Color color) const {
    std::unordered_set<Coord> pieces;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = board[i][j];
            if (piece != nullptr && piece->color == color) {
                pieces.insert(Coord{i, j});
            }
        }
    }
    return pieces;
}

Coord Board_Manager::get_king(Color color) const {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = board[i][j];
            if (piece != nullptr && piece->get_piece_name() == "KING" && piece->color == color) {
                return Coord{i, j};
            }
        }
    }
    throw std::runtime_error("King is missing");
}
#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include "board.h"

void slow_print(std::string text) {
    for (char c : text) {
        usleep(20000); // 1/50th of a second
        std::cout << c << std::flush;
    }
}

int main() {
    Board game;
    bool input_invalid;
    while (true) {
        game.display();

        // print this on reset if you screwed up the input
        if (input_invalid) {
            slow_print("INPUT INVALID, TRY AGAIN\n");
            input_invalid = false;
        }

        slow_print("CURRENT TURN: ");
        std::cout << C_BOLD;
        if (game.turn == WHITE) {
            std::cout << C_WHITE;
            slow_print("WHITE\n");
        } else {
            std::cout << C_BLACK;
            slow_print("BLACK\n");
        }
        std::cout << C_RESET;
        
        slow_print("SELECT A PIECE (SUCH AS 'A2') TO VIEW ITS MOVEMENT OPTIONS\n > ");
        std::string position;
        std::cin >> position;

        Coord selected_square = game.input_to_coord(position);
        
        // check if input is valid
        if (selected_square == NULLCOORD) {
            input_invalid = true;
            continue;
        }

        Piece* selected_piece = game.get_square(selected_square);

        std::unordered_set<Coord> options = selected_piece->possible_moves(game.board, selected_square);

        game.display(selected_square, options);


        std::string piece_name = typeid(*selected_piece).name();
        // remove trailing int and convert to uppercase
        piece_name = piece_name.substr(1);
        std::transform(piece_name.begin(), piece_name.end(), piece_name.begin(), ::toupper); 

        std::cout << piece_name;
        slow_print(" SELECTED");
        slow_print("ENTER A SQUARE TO MOVE, OR PRESS 'Q' TO QUIT\n > ");
        std::string target;
        std::cin >> target;
        if (target.length() == 1 && (target[0] == 'Q' || target[0] == 'q')) {
            continue;
        }
        Coord target_square = game.input_to_coord(target);
        if (target_square == NULLCOORD) {
            input_invalid = true;
            continue;
        }
    }
}
#include "local.h"

int local_game() {
    Board game;
    bool input_invalid;
    while (true) {
        game.display();

        if (game.king_in_check(game.turn)) {
            slow_print("YOUR KING IS IN CHECK\n");
        }

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
        
        slow_print("SELECT A PIECE (SUCH AS 'E2') TO VIEW ITS MOVEMENT OPTIONS\n > ");
        std::string position;
        std::cin >> position;

        Coord selected_square = game.input_to_coord(position);
        // check if input is valid
        if (selected_square == NULLCOORD) {
            input_invalid = true;
            continue;
        }

        // check if selected piece is a valid piece
        if (!game.valid_square(selected_square)) {
            input_invalid = true;
            continue;
        }
        std::unordered_set<Coord> options = game.get_possible_moves(selected_square);


        std::string piece_name = game.get_square_name(selected_square);
        Coord target_square;
        while (true) {
            game.display(selected_square, options);
            if (input_invalid) {
                slow_print("INPUT INVALID, TRY AGAIN\n");
                input_invalid = false;
            }
            slow_print(piece_name + " SELECTED\n");
            slow_print("ENTER A SQUARE TO MOVE, OR PRESS 'Q' TO QUIT\n > ");
            std::string target;
            std::cin >> target;
            if (target.length() == 1 && (target[0] == 'Q' || target[0] == 'q')) {
                break;
            }
                
            target_square = game.input_to_coord(target);
            if (target_square == NULLCOORD || options.count(target_square) == 0) {
                input_invalid = true;
                continue;
            }
            game.move_piece(selected_square, target_square);
            break;
        }
        if (game.pawn_promoted(target_square)) {
            while (true) {
                game.display();
                if (input_invalid) {
                    slow_print("INPUT INVALID, TRY AGAIN\n");
                    input_invalid = false;
                }
                slow_print("YOUR PAWN MAY BE PROMOTED. SELECT A PIECE\n");
                slow_print("QUEEN / BISHOP / KNIGHT / ROOK \n > ");
                std::string promo;
                std::cin >> promo;
                bool input_success = game.promote_pawn(target_square, promo);
                if (!input_success) {
                    input_invalid = true;
                } else {
                    break;
                }
            }
        }
    }
}
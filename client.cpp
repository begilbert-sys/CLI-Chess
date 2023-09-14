#include "client.h"
/*
messages are sent and received as 5-byte char arrays, such as "1303Q"
what each byte means:

1: select row 1
3: select column 3
2: move to row 0
3: move to column 3
Q: promote pawn to Queen

99% of the the time, the last byte will be '0', indicating that no pawn promotion took place 

When a checkmate occurs, the only byte sent will be 'C'
*/

void load_buffer(char buffer[MAX_PACKET_SIZE], Coord start, Coord end, std::string promo) {
    buffer[0] = start.row;
    buffer[1] = start.column;
    buffer[2] = end.row;
    buffer[3] = end.column;
    buffer[4] = promo[0];
}

std::string char_to_piece(char piece) {
    switch (piece) {
        case 'Q':
            return "QUEEN";
        case 'B':
            return "BISHOP";
        case 'K':
            return "KNIGHT";
        case 'R':
            return "ROOK";
        default:
            return "0";
    }
}

int client_game() {
    std::system("clear");
    slow_print("BEFORE THE GAME BEGINS, EXACTLY ONE COMPUTER MUST BE RUNNING THE SERVER\n");
    slow_print("ONCE THE SERVER HAS STARTED, ENTER 'C' TO CONNECT\n");
    std::string r;
    while (true){
        slow_print("\n > ");
        std::cin >> r;
        if (toupper(r[0]) == 'C') {
            break;
        }
    }
    slow_print("STARTING. . . ");

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_address;
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORT);
    serv_address.sin_addr.s_addr = INADDR_ANY;

    char buffer[MAX_PACKET_SIZE] = {INVITE_ACCEPT};
    char response[MAX_PACKET_SIZE];

    connect(client_socket, (struct sockaddr*)&serv_address, sizeof(serv_address));

    recv(client_socket, response, sizeof(response), 0);

    send(client_socket, buffer, sizeof(buffer), 0);

    Color side;
    if (response[0] == WHITE_INVITE) {
        std::cout << C_BOLD;
        slow_print("YOU ARE " + C_WHITE + "WHITE" + C_RESET + "\n");
        side = WHITE;
    }
    else if (response[0] == BLACK_INVITE) {
        slow_print("YOU ARE " + C_BLACK + "BLACK" + C_RESET + "\n");
        side = BLACK;
    } else {
        std::cout << response << std::endl;
        throw std::runtime_error("INVALID SERVER INITIALIZATION");
    }
    bool wait = false;
    if (side == WHITE) {
        slow_print("WAITING FOR CLIENT 2 TO JOIN. . . \n");
        recv(client_socket, response, sizeof(response), 0);
        assert(response[0] == READY);
    } else if (side == BLACK) {
        wait = true;
    }

    Board game;
    bool input_invalid = false;

    while (true) {
        if (wait) {
            game.display(side);
            slow_print("WAITING ON OPPONENT RESPONSE. . . ");
                
            recv(client_socket, response, sizeof(response), 0);

            Coord opp_start = Coord {response[0], response[1]};
            Coord opp_end = Coord {response[2], response[3]};
            char opp_promo_code = response[4];
            std::string opp_promo = char_to_piece(opp_promo_code);
            game.move_piece(opp_start, opp_end);
            if (opp_promo != "0") {
                game.promote_pawn(opp_end, opp_promo);
            }
        }
        wait = true;
        game.display(side);
        if (game.king_in_check(side)) {
            slow_print("YOUR KING IS IN CHECK\n");
        }
        // print this on reset if you screwed up the input
        if (input_invalid) {
            slow_print("INPUT INVALID, TRY AGAIN\n");
            input_invalid = false;
        }

        slow_print("CURRENT TURN: ");
        std::cout << C_BOLD;
        if (side == WHITE) {
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
            game.display(side, selected_square, options);
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
            std::string promo;
            while (true) {
                game.display(side);
                if (input_invalid) {
                    slow_print("INPUT INVALID, TRY AGAIN\n");
                    input_invalid = false;
                }
                slow_print("YOUR PAWN MAY BE PROMOTED. SELECT A PIECE\n");
                slow_print("QUEEN / BISHOP / KNIGHT / ROOK \n > ");
                promo = "";
                std::cin >> promo;
                bool input_success = game.promote_pawn(target_square, promo);
                if (!input_success) {
                    input_invalid = true;
                } else {
                    break;
                }
            }
            load_buffer(buffer, selected_square, target_square, promo);
        }
        else {
            load_buffer(buffer, selected_square, target_square, "0");
        }

        send(client_socket, buffer, sizeof(buffer), 0);
    }

    close(client_socket);

}
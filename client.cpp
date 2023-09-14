#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

#include "nw_consts.h"
#include "chess/board.h"
#include "slowprint.h"

int client_game() {
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

    int side;
    if (response[0] == WHITE_INVITE) {
        std::cout << "YOU ARE WHITE" << std::endl;
        side = WHITE;
    }
    else if (response[0] == BLACK_INVITE) {
        std::cout << "YOU ARE BLACK" << std::endl;
        side = BLACK;
    } else {
        std::cout << response << std::endl;
        throw std::runtime_error("INVALID SERVER INITIALIZATION");
    }

    if (side == WHITE) {
        std::cout << "WAITING FOR CLIENT 2 TO JOIN. . . " << std::endl;
        recv(client_socket, response, sizeof(response), 0);
        assert(response[0] == READY);
        std::cout << "enter something: " << std::endl;
        std::cin >> buffer;
        send(client_socket, buffer, sizeof(buffer), 0);
    }


    while (true) {
        recv(client_socket, response, sizeof(response), 0);

        std::cout << "OTHER CLIENT SAYS: " << response << std::endl << std::endl;

        std::cout << "enter something: " << std::endl;
        std::cin >> buffer;

        send(client_socket, buffer, sizeof(buffer), 0);
    }

    close(client_socket);

}
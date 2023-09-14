#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "nw_consts.h"

int server() {
    int serv_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_address;
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORT);
    serv_address.sin_addr.s_addr = INADDR_ANY;
    
    bind(serv_socket, (struct sockaddr*)&serv_address, sizeof(serv_address));

    listen(serv_socket, 2);
    std::cout << "SERVER IS LISTENING ON PORT " << PORT << std::endl;
    
    // initialize sockets
    sockaddr_in white_client_address;
    socklen_t white_client_length = sizeof(white_client_address);
    char white_buffer[MAX_PACKET_SIZE] = {WHITE_INVITE};
    char white_response[MAX_PACKET_SIZE];


    sockaddr_in black_client_address;
    socklen_t black_client_length = sizeof(black_client_address);
    char black_buffer[MAX_PACKET_SIZE] = {BLACK_INVITE};
    char black_response[MAX_PACKET_SIZE];


    // wait for first connection
    int white_client_socket = accept(serv_socket, (struct sockaddr*)&white_client_address, &white_client_length);
    send(white_client_socket, white_buffer, sizeof(white_buffer), 0);
    read(white_client_socket, white_response, sizeof(white_response));
    if (white_response[0] == INVITE_ACCEPT) {
        std::cout << "CLIENT 1 ACCEPTED INVITE" << std::endl;
    } else {
        throw new std::runtime_error("invalid client response");
    }
    std::cout << "CLIENT 1 CONNECTED. WAITING FOR CLIENT 2\n..." << std::endl;



    int black_client_socket = accept(serv_socket, (struct sockaddr*)&black_client_address, &black_client_length);
    send(black_client_socket, black_buffer, sizeof(black_buffer), 0);
    read(black_client_socket, black_response, sizeof(black_response));
    if (black_response[0] == INVITE_ACCEPT) {
        std::cout << "CLIENT 2 ACCEPTED INVITE" << std::endl;
    } else {
        throw new std::runtime_error("invalid client response");
    }
    white_buffer[0] = READY;
    send(white_client_socket, white_buffer, sizeof(white_buffer), 0);
    std::cout << "CLIENT 2 CONNECTED" << std::endl;


    while (true) {
        read(white_client_socket, white_response, sizeof(white_response));
        std::cout << "Received: " << white_response << std::endl;

        send(black_client_socket, white_response, strlen(white_response), 0);


        read(black_client_socket, black_response, sizeof(black_response));
        std::cout << "Received: " << black_response << std::endl;

        send(white_client_socket, black_response, strlen(black_response), 0);
    }
    close(white_client_socket);
    close(black_client_socket);
    close(serv_socket);

    return 0;

}
#ifndef CLIENT
#define CLIENT

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

#include "nw_consts.h"
#include "chess/board.h"
#include "slowprint.h"

int client_game();

#endif
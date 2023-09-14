#include "local.h"

/*
cd "/Users/bgilbert/Desktop/Chess/" && g++ -std=c++20 -I ./chess *.cpp chess/*.cpp -o main && "/Users/bgilbert/Desktop/Chess/"main
*/

int main() {
    slow_print("WELCOME TO CHESS!\n");
    slow_print("SELECT A MODE\n\n");
    slow_print("[1] OFFLINE TWO-PLAYER - PLAY ON THIS COMPUTER");
    slow_print("[2] ONLINE TWO-PLAYER - PLAY ON DIFFERENT COMPUTERS CONNECTED THRU LAN\n > ");
    std::string mode;
    std::cin >> mode;
    switch (mode[0]) {
        case '1':
            local_game();
    }
}
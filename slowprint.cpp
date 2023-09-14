#include "slowprint.h"

void slow_print(std::string text) {
    for (char c : text) {
        usleep(15000); // 0.015 seconds
        std::cout << c << std::flush;
    }
}

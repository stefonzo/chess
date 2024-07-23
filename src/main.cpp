#include <stdio.h>
#include "game.h"

#define APP_NAME "Chess v0.01"

int main() {
    printf("Chess v0.01\n");
    game chess(WINDOW_WIDTH, WINDOW_HEIGHT, "Chess v0.01");
    chess.loop();
    return 0;
}
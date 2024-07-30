#include <stdio.h>
#include "game.h"

#define APP_NAME "Chess v0.02"

int main() {
    printf("Chess v0.02\n");
    game chess(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);
    chess.loop();
    return 0;
}
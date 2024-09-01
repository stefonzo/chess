#include "chess.h"

void chess_board::init_chess_board(void) {
    /*
        Initialize chess board
    */
    white_pieces = 16, black_pieces = 16;
    total_pieces = white_pieces + black_pieces;
    number_of_white_moves = 0, number_of_black_moves = 0, number_of_turns = 0;

    // initialize player info
    player_white = {"Player White", false, false, false, false, true, piece_color::white};
    player_black = {"Player Black", false, false, false, false, false, piece_color::black};
}

chess_board::chess_board() {
    init_chess_board();
}

chess_board::~chess_board() {

}

void chess_board::print_board_info(void) {
    printf("White has made %d moves. Black has made %d moves.\n", number_of_white_moves, number_of_black_moves);
    printf("%d turns have elapsed.\n", number_of_turns);
    printf("%d total pieces remaining.\n", total_pieces);
    printf("%d white pieces remaining. %d Black pieces remaining.\n", white_pieces, black_pieces);
}
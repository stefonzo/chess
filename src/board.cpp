#include "board.h"

void chess_board::init_chess_board(void) {
    // initialize pieces array

    //empty squares
    for (unsigned i = a3; i <= h6; i++) {
        pieces[i].is_empty = empty::yes;
    }
    // white pawns
    for (unsigned i = a2; i <= h2; i++) {
        pieces[i] = {piece_color::white, piece_type::pawn, empty::no, false};
    }
    // black pawns
    for (unsigned i = a7; i <= h7; i++) {
        pieces[i] = {piece_color::black, piece_type::pawn, empty::no, false};
    }
    // first row
    pieces[a1] = {piece_color::white, piece_type::rook, empty::no, false};
    pieces[b1] = {piece_color::white, piece_type::knight, empty::no, false};
    pieces[c1] = {piece_color::white, piece_type::bishop, empty::no, false};
    pieces[d1] = {piece_color::white, piece_type::queen, empty::no, false};
    pieces[e1] = {piece_color::white, piece_type::king, empty::no, false};
    pieces[f1] = {piece_color::white, piece_type::bishop, empty::no, false};
    pieces[g1] = {piece_color::white, piece_type::knight, empty::no, false};
    pieces[h1] = {piece_color::white, piece_type::rook, empty::no, false};

    // last row
    pieces[a8] = {piece_color::black, piece_type::rook, empty::no, false};
    pieces[b8] = {piece_color::black, piece_type::knight, empty::no, false};
    pieces[c8] = {piece_color::black, piece_type::bishop, empty::no, false};
    pieces[d8] = {piece_color::black, piece_type::queen, empty::no, false};
    pieces[e8] = {piece_color::black, piece_type::king, empty::no, false};
    pieces[f8] = {piece_color::black, piece_type::bishop, empty::no, false};
    pieces[g8] = {piece_color::black, piece_type::knight, empty::no, false};
    pieces[h8] = {piece_color::black, piece_type::rook, empty::no, false};
}

chess_board::chess_board() {
    init_chess_board();
    player_white = {"White", false, false, false, false, false, 0, piece_color::white};
    player_black = {"Black", false, false, false, false, false, 0, piece_color::black};
}

chess_board::~chess_board() {

}
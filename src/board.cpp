#include "board.h"

unsigned get_index(unsigned rank, unsigned file) {
    if ((file < 0 || file > 7)|| (rank < 0 || rank > 7)) {
        return NUM_SQUARES + 1;
    } else {
        return rank * 8 + file;
    }
}

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

    // initialize player info
    player_white = {"Player White", false, false, false, false, true, piece_color::white};
    player_black = {"Player Black", false, false, false, false, false, piece_color::black};
}

bool chess_board::check_move(move m) {
    if ((m.from >= NUM_SQUARES || m.from < 0) || (m.to >= NUM_SQUARES || m.to < 0)) { // is move within board index?
        return false;
    }
    switch (pieces[m.from].is_empty) {
        case empty::no: // there is a piece in the from square, what do we do with it?
            switch (pieces[m.from].color) {
                case piece_color::white:
                    switch (pieces[m.from].type) {
                            case piece_type::pawn:
                                switch (pieces[m.from].has_moved) {
                                    case false:
                                    break;

                                    case true:
                                        // check to see if pawn can attack
                                        
                                    break;
                                }
                            break;

                            case piece_type::rook:
                            break;

                            case piece_type::knight:
                            break;

                            case piece_type::bishop:
                            break;

                            case piece_type::king:
                            break;

                            case piece_type::queen:
                            break;
                        }
                break;

                case piece_color::black:
                    switch (pieces[m.from].type) {
                            case piece_type::pawn:
                                switch (pieces[m.from].has_moved) {
                                    case false:
                                    break;

                                    case true:
                                        
                                    break;
                                }
                            break;

                            case piece_type::rook:
                            break;

                            case piece_type::knight:
                            break;

                            case piece_type::bishop:
                            break;

                            case piece_type::king:
                            break;

                            case piece_type::queen:
                            break;
                        }
                break;
            }
        break;
        case empty::yes:
            return false;
        break;
    }
}
 
bool chess_board::move_piece(move m) { // simply moves pieces (does not check if move is valid, use check_move() for that)
    chess_piece fill;
    fill.is_empty = empty::yes;
    if ((m.from >= NUM_SQUARES || m.from < 0) || (m.to >= NUM_SQUARES || m.to < 0)) { // does index even exist inside board?
        return false;
    }
    if (player_white.turn) { // white's moves
        switch (pieces[m.from].is_empty) {
            case empty::no:
                switch (pieces[m.from].color) {
                    case piece_color::white:
                        switch (pieces[m.from].type) {
                            case piece_type::pawn:
                            break;

                            case piece_type::rook:
                            break;

                            case piece_type::knight:
                            break;

                            case piece_type::bishop:
                            break;

                            case piece_type::king:
                            break;

                            case piece_type::queen:
                            break;
                        }
                    break;

                    case piece_color::black:
                        return false;
                    break;
                }
            break;

            case empty::yes:
                return false;
            break;
        }
    } else if (player_black.turn) { // black's moves
        switch (pieces[m.from].is_empty) {
            case empty::no:
                switch (pieces[m.from].color) {
                    case piece_color::black:
                        switch (pieces[m.from].type) {
                            case piece_type::pawn:
                            break;

                            case piece_type::rook:
                            break;

                            case piece_type::knight:
                            break;

                            case piece_type::bishop:
                            break;

                            case piece_type::king:
                            break;

                            case piece_type::queen:
                            break;
                        }
                    break;

                    case piece_color::white:
                        return false;
                    break;
                }
            break;

            case empty::yes:
                return false;
            break;
        }
    }
}

chess_board::chess_board() {
    init_chess_board();
}

chess_board::~chess_board() {

}
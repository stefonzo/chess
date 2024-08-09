#include "board.h"

unsigned get_index(unsigned rank, unsigned file) {
    if ((file > 7)|| (rank > 7)) {
        return NUM_SQUARES + 1;
    } else {
        return rank * 8 + file;
    }
}

void chess_board::init_chess_board(void) {
    white_pieces = 16, black_pieces = 16;
    total_pieces = white_pieces + black_pieces;
    number_of_white_moves = 0, number_of_black_moves = 0, number_of_turns = 0;
    /*
        Initialize chess board
    */
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
    player_white = {"Player White", false, false, false, false, true, piece_color::white, {-1, -1}};
    player_black = {"Player Black", false, false, false, false, false, piece_color::black, {-1, -1}};
}

bool chess_board::can_white_pawn_move(move m) {
    // move two squares forward
    if ((pieces[m.from].has_moved == false) && (m.to == m.from + (2 * 8))) { // pawn hasn't been moved and is going to be moved 2 squares north
        // check if two squares north of piece are occupied
        for (int s = 8; s <= 16; s = s * 2) {
            if (pieces[m.from + s].is_empty == empty::no) return false;
        }
        pieces[m.from].has_moved = true;
        return true;
    }
    // move one square north
    if ((m.to == m.from + 8) && (pieces[m.to].is_empty == empty::yes)) return true;
    // standard attack
    if (((m.to == m.from + 9) || (m.to == m.from + 7)) && (pieces[m.to].is_empty == empty::no)) return true; 
    return false; 
}

bool chess_board::can_white_rook_move(move m) { return false;}
bool chess_board::can_white_bishop_move(move m) { return false; }
bool chess_board::can_white_knight_move(move m) { return false; }
bool chess_board::can_white_king_move(move m) {  return false; }
bool chess_board::can_white_queen_move(move m) { return false; }

bool chess_board::can_black_pawn_move(move m) {
    // move two squares down
    if ((pieces[m.from].has_moved == false) && (m.to == m.from + (2 * - 8))) {
        // check if two squares south of piece are occupied
        for (int s = -9; s <= -18 ;s = s * 2) {
            if (pieces[m.from + s].is_empty == empty::no) return false;
        }
        pieces[m.from].has_moved = true;
        return true;
    }
    // move one square south
    if ((m.to == m.from - 8) && (pieces[m.to].is_empty == empty::yes)) return true;
    // standard attack 
    if (((m.to == m.from - 9) || (m.to == m.from - 7)) && (pieces[m.to].is_empty == empty::no)) return true; 
    return false;
}

bool chess_board::can_black_rook_move(move m) { return false; }
bool chess_board::can_black_bishop_move(move m) { return false; }
bool chess_board::can_black_knight_move(move m) { return false;}
bool chess_board::can_black_king_move(move m) { return false; }
bool chess_board::can_black_queen_move(move m) { return false; }

bool chess_board::check_move(move m) { // (figure out how to use macros here)
    if (m.from == m.to) return false; // not likely to be a computer generated move but in case the user has a move like this
    if ((m.from >= NUM_SQUARES) || (m.from < 0) || (m.to >= NUM_SQUARES) || (m.to < 0)) return false; // is move within board index?
    // code for conditionals comes from chat gpt
    // unsigned from_file = m.from % 8;
    // unsigned from_rank = m.from / 8;
    // unsigned to_rank = m.to / 8;

    // preventing horizontal wraparounds 
    // if ((m.to == m.from + 1) && (from_file == 7)) return false; // moving to the east from the far right rank
    // if ((m.to == m.from - 1) && (from_file == 0)) return false; // moving to the west from the far left rank

    // // preventing vertical wraparounds (this code is kind of redundant)
    // if ((m.to == m.from + 8) && (to_rank == from_rank + 1)) return false; // in last rank and try to move north
    // if ((m.to == m.from - 8) && (to_rank == from_rank - 1)) return false; // in first rank and try to move south

    // // preventing diagonal wraparounds (my code)
    if ((m.to == m.from + 9) && (m.to > h8)) return false; // in upper right corner trying to move northeast
    if ((m.to == m.from + 7) && (m.from == a8)) return false; // in upper left corner trying to move northwest
    if ((m.to == m.from - 9) && (m.from == a1)) return false;  // in bottom left corner trying to move southwest
    if ((m.to == m.from - 7) && (m.from == h1)) return false; // in bottom right corner trying to move southeast

    chess_piece piece = pieces[m.from];
    switch (piece.is_empty) {
        case empty::no:
            switch (piece.color) {
                case piece_color::white:
                    switch (piece.type) {
                        case piece_type::pawn:
                            return can_white_pawn_move(m);
                        break;

                        case piece_type::rook:
                            return can_white_rook_move(m);
                        break;

                        case piece_type::knight:
                            return can_white_knight_move(m);
                        break;

                        case piece_type::bishop:
                            return can_white_bishop_move(m);
                        break;
                        
                        case piece_type::king:
                            return can_white_king_move(m);
                        break;

                        case piece_type::queen:
                            return can_white_queen_move(m);
                        break;
                    }
                break;

                case piece_color::black:
                    switch (piece.type) {
                        case piece_type::pawn:
                            return can_black_pawn_move(m);
                        break;

                        case piece_type::rook:
                            return can_black_rook_move(m);
                        break;

                        case piece_type::knight:
                            return can_black_knight_move(m);
                        break;

                        case piece_type::bishop:
                            return can_black_bishop_move(m);
                        break;
                        
                        case piece_type::king:
                            return can_black_king_move(m);
                        break;

                        case piece_type::queen:
                            return can_black_queen_move(m);
                        break;
                    }
                break;
            }
        break;
        case empty::yes:
            return false;
        break;
    }
    return false;
}
 
bool chess_board::move_piece(move m) { // simply moves pieces (does not check if move is valid, use check_move() for that)
    chess_piece swap_piece = {piece_color::white, piece_type::pawn, empty::yes, false};
    printf("Move from %d to %d\n", m.from, m.to);
    if (check_move(m)) {
        if (player_white.turn) {
            if (pieces[m.from].color == piece_color::white) {
                // check for capture
                if (pieces[m.to].color == piece_color::black) {
                    black_pieces--;
                    total_pieces--;
                }
                pieces[m.to] = pieces[m.from];
                pieces[m.from] = swap_piece;
                player_white.turn = false;
                player_black.turn = true;
                return true;
            } 
        } else if (player_black.turn) {
            if (pieces[m.from].color == piece_color::black) {
                // check for capture
                if (pieces[m.to].color == piece_color::white) {
                    white_pieces--;
                    total_pieces--;
                }
                pieces[m.to] = pieces[m.from];
                pieces[m.from] = swap_piece;
                player_white.turn = true;
                player_black.turn = false;
                return true;
            }
        }
    }
    return false;
}

unsigned chess_board::get_turns(void) {
    return number_of_turns;
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
#pragma once

enum class piece_color {
    white, black
};

enum class piece_type {
    pawn, rook, knight, bishop, queen, king
};

enum class empty {
    no, yes
};

struct chess_piece {
    piece_color color;
    piece_type type;
    empty is_empty;
    bool has_moved; // used with king and rook for castling
};
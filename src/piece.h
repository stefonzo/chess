#pragma once

enum class piece_color {
    white, black
};

enum class piece_type {
    pawn, rook, knight, bishop, queen, king, empty
};

struct chess_piece {
    piece_color color;
    piece_type type;
    bool has_moved; // used with king and rook for castling
};
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

enum class piece_color {
    white, black
};

enum class piece_type {
    pawn, rook, knight, bishop, queen, king
};

class chess_piece {
    private:
        piece_color color;
        piece_type type;
    public:
        chess_piece(piece_color c, piece_type t);
        piece_color get_piece_color(void);
        piece_type get_piece_type(void);
};
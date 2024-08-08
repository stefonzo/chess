#pragma once

#include "piece.h"
#include <bitset>
#include <vector>
#include <stdint.h>

#define NUM_SQUARES 64

// used for board indexing
#define a1 0 
#define b1 1
#define c1 2
#define d1 3
#define e1 4
#define f1 5
#define g1 6
#define h1 7

#define a2 8 
#define b2 9
#define c2 10
#define d2 11
#define e2 12
#define f2 13
#define g2 14
#define h2 15

#define a3 16 
#define b3 17
#define c3 18
#define d3 19
#define e3 20
#define f3 21
#define g3 22
#define h3 23

#define a4 24 
#define b4 25
#define c4 26
#define d4 27
#define e4 28
#define f4 29
#define g4 30
#define h4 31

#define a5 32 
#define b5 33
#define c5 34
#define d5 35
#define e5 36
#define f5 37
#define g5 38
#define h5 39

#define a6 40 
#define b6 41
#define c6 42
#define d6 43
#define e6 44
#define f6 45
#define g6 46
#define h6 47

#define a7 48 
#define b7 49
#define c7 50
#define d7 51
#define e7 52
#define f7 53
#define g7 54
#define h7 55

#define a8 56 
#define b8 57
#define c8 58
#define d8 59
#define e8 60
#define f8 61
#define g8 62
#define h8 63

// board directions
#define NORTH +8
#define NORTHEAST +9
#define NORTHWEST +7
#define SOUTH -8
#define SOUTHEAST -7
#define SOUTHWEST -9
#define EAST +1
#define WEST -1

struct move {
    int from, to;
};

struct player {
    std::string name;
    bool resigned, check, checkmate, draw, turn;
    piece_color color;
    move player_move;
};

// this class will have to have all the information needed to represent the state of a chess game
// it will be used in move finding heavily (pointers? shared_ptr?)
class chess_board {
    public:
        // player data used to set/get additional game state data
        player player_white, player_black;

        unsigned number_of_white_moves, number_of_black_moves, number_of_turns;
        unsigned white_pieces, black_pieces, total_pieces;

        chess_board();
        ~chess_board();

        // methods
        void init_chess_board(void);

        // piece movement functions
        bool can_white_pawn_move(move m);
        bool can_white_rook_move(move m);
        bool can_white_bishop_move(move m);
        bool can_white_knight_move(move m);
        bool can_white_king_move(move m);
        bool can_white_queen_move(move m);

        bool can_black_pawn_move(move m);
        bool can_black_rook_move(move m);
        bool can_black_bishop_move(move m);
        bool can_black_knight_move(move m);
        bool can_black_king_move(move m);
        bool can_black_queen_move(move m);

        bool check_move(move m);
        std::vector<move> generate_moves(move m);
        bool move_piece(move m);
        void print_board_info(void);

        // square-centric board representation (array, mailbox)
        chess_piece pieces[NUM_SQUARES];

        // piece-centric board representation (bitset)
        std::bitset<NUM_SQUARES> white_pawn;
        std::bitset<NUM_SQUARES> white_rook;
        std::bitset<NUM_SQUARES> white_knight;
        std::bitset<NUM_SQUARES> white_bishop;
        std::bitset<NUM_SQUARES> white_king;
        std::bitset<NUM_SQUARES> white_queen;

        std::bitset<NUM_SQUARES> black_pawn;
        std::bitset<NUM_SQUARES> black_rook;
        std::bitset<NUM_SQUARES> black_knight;
        std::bitset<NUM_SQUARES> black_bishop;
        std::bitset<NUM_SQUARES> black_king;
        std::bitset<NUM_SQUARES> black_queen;
};

// methods for the chess_board
unsigned get_index(unsigned rank, unsigned file);
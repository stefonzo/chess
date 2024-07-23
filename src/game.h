#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <memory>

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define MAIN_MENU_CHAR_WIDTH 15
#define SETTINGS_MENU_CHAR_WIDTH 30


enum class GAME_STATE {
    MENU,
    GAME,
    SETTINGS,
    QUIT
};

enum class ITEM_SETTINGS {
    MENU
};

// TODO: implement a better menu system...

class menu {
        unsigned items;
    public:
        std::vector<std::string> menu_items;
        std::vector<SDL_Rect> item_boundaries;
        std::vector<SDL_Texture*> item_textures;
        std::vector<SDL_Texture*> selected_item_textures;
        std::vector<bool> item_checked;
        std::vector<GAME_STATE> item_game_state;
        std::vector<ITEM_SETTINGS> item_settings;
        menu(unsigned n_items);
        void cleanup_menu(void);
        unsigned get_items(void) { return items; }
        bool check_button(int mouse_x, int mouse_y, SDL_Rect *button);
};

class game {
    private:
        // SDL data
        SDL_Window *game_window = NULL;
        SDL_Renderer *game_renderer = NULL;

        // ttf data
        TTF_Font *game_font;
        SDL_Color main_color, selected_color;
        SDL_Surface *font_surface;
        SDL_Texture *version_info;
        SDL_Rect display_version_info;
        std::string VERSION;

        // window data
        unsigned game_window_width, game_window_height;
        std::string game_window_title;

        // mouse data
        int mouse_x, mouse_y;
        bool mouse_clicked;

        // keyboard data
        char key_press;

        // init methods
        void init_sdl(void);
        void init_main_menu(void);
        void init_settings_menu(void);
        void init_game(void);

        void cleanup_sdl(void);
        void cleanup_game(void);

        // input functions
        void mouse_event(SDL_Event *e);
        void keyboard_event(SDL_Event *e);

        void update_main_menu(void);
        void render_main_menu(void);

        void update_settings_menu(void);
        void render_settings_menu(void);

        // main game loop 
        void update_game(void);
        void render_game(void);

        GAME_STATE game_state;

        std::unique_ptr<menu> main_menu;
        std::unique_ptr<menu> settings_menu;
    public:
        game(unsigned window_width, unsigned window_height, std::string window_title);
        ~game();
        void loop(void); // main game loop
};
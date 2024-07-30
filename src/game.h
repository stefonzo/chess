#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>

#define FPS 60
#define FRAME_DELAY 1000 / FPS
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define MAIN_MENU_CHAR_WIDTH 15
#define SETTINGS_MENU_CHAR_WIDTH 25
#define SPLASHSCREEN_TIME 5000.0f
#define SPLASHSCREEN_FONT_WIDTH 20
#define SPLASHSCREEN_FONT_HEIGHT 55
#define SPLASHSCREEN_X 250

enum class GAME_STATE {
    SPLASH_SCREEN,
    MENU,
    GAME,
    SETTINGS,
    QUIT
};

enum class ITEM_SETTINGS {
    MENU
};

/*  TODO
        -begin working on implementing chess data/logic 
        -get textures for chess pieces rendering into game
        -implement animation with interpolation and or matrices for animation (ie: rotating, shrinking, stretching textures)
*/

class timer {
    private:
        bool paused; // is the timer running?
        std::chrono::time_point<std::chrono::steady_clock> start_time, paused_time; // time_point type represents a point in time obtained from a steady_clock, these values are used to calculate durations (alongside paused duration)
        std::chrono::duration<double> paused_duration;
    public:
        timer();
        ~timer();
        double get_time(void);
        void reset_timer(void);
        void start_timer(void);
        void pause_timer(void);
};

class texture_manager {
    private:
        SDL_Surface *texture_surface; // temporary surface used in texture generation
        std::unordered_map<std::string, SDL_Texture*> textures; // hashmap with string keys that stores textures
        std::vector<std::string> texture_names; // this holds the textures string keys but I need to get rid of this and figure out a different system for managing the hash keys
        void cleanup_textures(void);
    public:
        std::unordered_map<std::string, SDL_Rect> texture_quads; // this is public so I can change the x y coordinates for texture rendering
        texture_manager();
        ~texture_manager();
        void add_texture(std::string path, std::string texture_name, SDL_Renderer *r);
        void remove_texture(std::string texture_name);
        SDL_Texture* get_texture(std::string texture_name); // this method will be used in rendering to the game window
};

class menu {
    private:
        unsigned items;
        void cleanup_menu(void);
    public:
        std::vector<std::string> menu_items; // holds all the button text
        std::vector<SDL_Rect> item_boundaries; // boundaries for the menu buttons/items
        std::vector<SDL_Texture*> item_textures; // holds ttf textures for the menu items
        std::vector<SDL_Texture*> selected_item_textures; // ttf textures of item text in a different color for highlight selection
        std::vector<bool> item_checked; // is the mouse cursor over a button?
        std::vector<GAME_STATE> item_game_state; // buttons/items are associated with different game states
        std::vector<ITEM_SETTINGS> item_settings; // for buttons/items in the settings menu
        menu(unsigned n_items); // n_items is how many buttons/items are in the menu
        ~menu(); 
        unsigned get_items(void) { return items; } // used for loops
        bool check_button(int mouse_x, int mouse_y, SDL_Rect *button);
};

class game {
    private:
        // frame data
        int frame_start, frame_time;
        // SDL data
        SDL_Window *game_window = NULL;
        SDL_Renderer *game_renderer = NULL;

        // ttf data
        TTF_Font *game_font;
        SDL_Color main_color, selected_color;
        SDL_Surface *font_surface;
        SDL_Rect display_version_info;
        std::string VERSION;

        // window data
        unsigned game_window_width, game_window_height;
        std::string game_window_title;
        SDL_Texture *version_info;

        // mouse data
        int mouse_x, mouse_y;
        bool mouse_clicked;

        // keyboard data
        char key_press;

        // texture data
        std::unique_ptr<texture_manager> game_texture_manager;

        // splashscreen data
        double splashscreen_time;
        std::unique_ptr<timer> splashscreen_timer;
        SDL_Texture *splashscreen_texture;
        SDL_Rect splashscreen_text_quad;
        std::string SPLASHSCREEN_TEXT;

        // init methods
        void init_sdl(void);
        void init_splashscreen(void);
        void init_main_menu(void);
        void init_settings_menu(void);
        void init_game(void);

        void cleanup_sdl(void);
        void cleanup_game(void);

        // input functions
        void mouse_event(SDL_Event *e);
        void keyboard_event(SDL_Event *e);

        void update_splash_screen(double dt);
        void render_splash_screen(void);

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
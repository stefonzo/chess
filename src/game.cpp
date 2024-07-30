#include "game.h"

/*
    Timer Code
*/

timer::timer() {
    paused = false;
    paused_duration = std::chrono::duration<double>(0);
}

timer::~timer() {} // the destructor is currently unused...

void timer::start_timer(void) {
    if (paused == true) {
        std::chrono::time_point<std::chrono::steady_clock> resumed_time = std::chrono::steady_clock::now();
        paused_duration += resumed_time - paused_time;
        paused = false;
    } else {
        start_time = std::chrono::steady_clock::now();
        paused_duration = std::chrono::duration<double>::zero();
    }
}

void timer::pause_timer(void) {
    paused_time = std::chrono::steady_clock::now();
    paused = true;
}

double timer::get_time(void) {
    if (paused == true) {
        return paused_duration.count();
    } else {
        std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();
        return (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time) + paused_duration).count();
    }
}

void timer::reset_timer(void) {
    start_time = std::chrono::steady_clock::now();
    paused_duration = std::chrono::duration<double>::zero();
    paused = false;
}

/*
    Menu Code
*/

menu::menu(unsigned n_items) {
    items = n_items;
    menu_items.resize(items);
    item_boundaries.resize(items);
    item_textures.resize(items);
    item_checked.resize(items);
    selected_item_textures.resize(items);
    item_game_state.resize(items);
    item_settings.resize(items);
}

menu::~menu() {
    cleanup_menu();
}

void menu::cleanup_menu(void) {
    for (unsigned i = 0; i < items; i++) {
        if (item_textures[i] != NULL) {
            SDL_DestroyTexture(item_textures[i]);
            item_textures[i] = NULL;
        }
    }

    for (unsigned i = 0; i < items; i++) {
        if (selected_item_textures[i] != NULL) {
            SDL_DestroyTexture(selected_item_textures[i]);
            selected_item_textures[i] = NULL;
        }
    }
}

bool menu::check_button(int mouse_x, int mouse_y, SDL_Rect *button) {
    if (mouse_x > button->x && mouse_x < button->w + button->x && mouse_y > button->y && mouse_y < button->h+button->y)
    {
        return true;
    }
    return false;
}

/*
    Texture Manager Code
*/

texture_manager::texture_manager() {
    textures.clear(); // redundant but I want my constructor to do something for now :p
    texture_quads.clear();
}

void texture_manager::cleanup_textures(void) {
    for (auto it = textures.begin(); it != textures.end(); it++) {
        SDL_DestroyTexture(it->second);
        it->second = NULL;
    }
}

texture_manager::~texture_manager() {
    cleanup_textures();
}

void texture_manager::add_texture(std::string path, std::string texture_name, SDL_Renderer *r) {
    texture_names.push_back(texture_name);
    // create surface from png
    SDL_Surface *loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }

    // create texture from surface
    printf("creating texture\n");
    textures.insert({texture_name, SDL_CreateTextureFromSurface(r, loaded_surface)});
    if (textures[texture_name] == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
    } 

    SDL_Rect temp_rect;
    temp_rect.w = loaded_surface->w;
    temp_rect.h = loaded_surface->h;
    texture_quads.insert({texture_name, temp_rect});
    

    // get rid of loaded surface
    SDL_FreeSurface(loaded_surface);
}

void texture_manager::remove_texture(std::string texture_name) {
    // todo use queue instead of vector to store strings of texture names
    SDL_DestroyTexture(textures[texture_name]);
    textures[texture_name] = NULL;
    textures.erase(texture_name);
    texture_quads.erase(texture_name);
}

SDL_Texture* texture_manager::get_texture(std::string texture_name) {
    return textures[texture_name];
}

/*
    Game Code
*/

/*
    Init Code
*/

void game::init_game(void) {
    printf("Intializing chess...\n");

    game_state = GAME_STATE::SPLASH_SCREEN; // testing splashscreen atm

    // initialize text color
    main_color = {0, 0, 0, 255};

    // initialize text select color
    selected_color = {255, 255, 255, 255};

    background_color = {0, 220, 50, 255};
    
    VERSION = "Version 0.02";

    // initialize version info texture
    font_surface = TTF_RenderText_Solid(game_font, VERSION.c_str(), main_color);
    if (font_surface == NULL) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    version_info = SDL_CreateTextureFromSurface(game_renderer, font_surface);
    if (version_info == NULL) {
        printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(font_surface);

    // initialize rectangle for displaying version info
    display_version_info.x = 10;
    display_version_info.y = WINDOW_HEIGHT - 30;
    display_version_info.w = VERSION.length() * MAIN_MENU_CHAR_WIDTH;
    display_version_info.h = 25;

    game_texture_manager = std::make_unique<texture_manager>();

    init_main_menu();
    init_settings_menu();
    init_splashscreen();
    init_board();
}

void game::init_board(void) {
    // initialize boundary for chess board
    board_boundary = {350, 100, BOARD_WIDTH + 2, BOARD_HEIGHT + 2}; 
    game_texture_manager->add_texture("chess_board.png", "chess_board", game_renderer);
    // initialize chess board quad rectangles
    game_texture_manager->texture_quads["chess_board"].x = BOARD_X;
    game_texture_manager->texture_quads["chess_board"].y = BOARD_Y;
}

void game::init_sdl(void) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        printf("SDL could not intialize! SDL Error: %s\n", SDL_GetError());
    
    // set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        printf("Warning: Linear texture filtering not enabled!\n");
    
    // create window
    game_window = SDL_CreateWindow(game_window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, game_window_width, game_window_height, SDL_WINDOW_SHOWN);
    if (game_window == NULL)
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    
    // create renderer for window
    game_renderer = SDL_CreateRenderer(game_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (game_renderer == NULL)
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    
    // initialize renderer color
    SDL_SetRenderDrawColor(game_renderer, 0, 0, 99, 0xFF);

    // initialize PNG loading
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        printf("SDL_image could not intialize! SDL_image Error: %s\n", IMG_GetError());
    }

    // initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

    game_font = TTF_OpenFont("test.ttf", 16);
    if (game_font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

void game::init_splashscreen(void) {
    game_texture_manager->add_texture("splashscreen.png", "splashscreen_texture", game_renderer);
    game_texture_manager->texture_quads["splashscreen_texture"].x = SPLASHSCREEN_X;
    game_texture_manager->texture_quads["splashscreen_texture"].y = 40;

    // generate texture for text that will be displayed in splash screen
    SPLASHSCREEN_TEXT = "Welcome to the game of Chess!";
    splashscreen_text_quad.x = SPLASHSCREEN_X + 30;
    splashscreen_text_quad.y = WINDOW_HEIGHT - 80;
    splashscreen_text_quad.h = SPLASHSCREEN_FONT_WIDTH;
    splashscreen_text_quad.w = SPLASHSCREEN_TEXT.length() * SPLASHSCREEN_FONT_WIDTH;

    font_surface = TTF_RenderText_Solid(game_font, SPLASHSCREEN_TEXT.c_str(), main_color);
    if (font_surface == NULL) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    splashscreen_texture = SDL_CreateTextureFromSurface(game_renderer, font_surface);
    if (splashscreen_texture == NULL) {
        printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(font_surface);

    // init splashscreen_timer
    splashscreen_timer = std::make_unique<timer>();
    splashscreen_timer->reset_timer();
    splashscreen_timer->start_timer();
    printf("First time is: %G ms\n", splashscreen_timer->get_time());
}

void game::init_main_menu(void) {
    mouse_clicked = false;
    main_menu = std::make_unique<menu>(3);

    // initialize main menu text (crash occurs here)
    main_menu->menu_items[0] = "Play";
    main_menu->menu_items[1] = "Settings";
    main_menu->menu_items[2] = "Exit";

    // associate game states with different buttons
    main_menu->item_game_state[0] = GAME_STATE::GAME;
    main_menu->item_game_state[1] = GAME_STATE::SETTINGS;
    main_menu->item_game_state[2] = GAME_STATE::QUIT;

    // initialize main menu textures
    for (unsigned i = 0; i < main_menu->get_items(); i++) {
        font_surface = TTF_RenderText_Solid(game_font, main_menu->menu_items[i].c_str(), main_color);
        if (font_surface == NULL) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        }
        main_menu->item_textures[i] = SDL_CreateTextureFromSurface(game_renderer, font_surface);
        if (main_menu->item_textures[i] == NULL) {
            printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(font_surface);
    }

    // initialize main menu rectangles
    for (unsigned i = 0; i < main_menu->get_items(); i++) {
        main_menu->item_boundaries[i].x = (WINDOW_WIDTH / 2) - 45;
        main_menu->item_boundaries[i].y = (i * 45) + 300;
        main_menu->item_boundaries[i].w = main_menu->menu_items[i].length() * MAIN_MENU_CHAR_WIDTH;
        main_menu->item_boundaries[i].h = 30;
    }

    // initialize main menu item checked
    for (unsigned i = 0; i < main_menu->get_items(); i++) {
        main_menu->item_checked[i] = false;
    }
    // initialize selected items textures
    for (unsigned i = 0; i < main_menu->get_items(); i++) {
        font_surface = TTF_RenderText_Solid(game_font, main_menu->menu_items[i].c_str(), selected_color);
        if (font_surface == NULL) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        }
        main_menu->selected_item_textures[i] = SDL_CreateTextureFromSurface(game_renderer, font_surface);
        if (main_menu->selected_item_textures[i] == NULL) {
            printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(font_surface);
    }
}

void game::init_settings_menu(void) {

   settings_menu = std::make_unique<menu>(1); // for now there aren't any settings until I begin programming the game...
   settings_menu->menu_items[0] = "Main Menu";
   settings_menu->item_settings[0] = ITEM_SETTINGS::MENU;
   
   // initialize textures for settings menu
   for (unsigned i = 0; i < settings_menu->get_items(); i++) {
        font_surface = TTF_RenderText_Solid(game_font, settings_menu->menu_items[i].c_str(), main_color);
        if (font_surface == NULL) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        }
        settings_menu->item_textures[i] = SDL_CreateTextureFromSurface(game_renderer, font_surface);
        if (settings_menu->item_textures[i] == NULL) {
            printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(font_surface);
   }

   // initialize selected textures

   for (unsigned i = 0; i < settings_menu->get_items(); i++) {
        font_surface = TTF_RenderText_Solid(game_font, settings_menu->menu_items[i].c_str(), selected_color);
        if (font_surface == NULL) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        }
        settings_menu->selected_item_textures[i] = SDL_CreateTextureFromSurface(game_renderer, font_surface);
        if (settings_menu->selected_item_textures[i] == NULL) {
            printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(font_surface);
   }

   // initialize rectangles for settings menu items
   for (unsigned i = 0; i < settings_menu->get_items(); i++) {
        settings_menu->item_boundaries[i].x = (WINDOW_WIDTH / 2) - 45;
        settings_menu->item_boundaries[i].y = (i * 45) + 300;
        settings_menu->item_boundaries[i].w = main_menu->menu_items[i].length() * SETTINGS_MENU_CHAR_WIDTH;
        settings_menu->item_boundaries[i].h = 30;
   }

   // initialize if settings are checked (false by default) (put this in constructor...)
   for (unsigned i = 0; i < settings_menu->get_items(); i++) {
        settings_menu->item_checked[i] = false;
   }
}

/*
    Cleanup Code
*/
void game::cleanup_game(void) {
    // cleaning up some ttf text
    if (version_info != NULL) {
        SDL_DestroyTexture(version_info);
        version_info = NULL;
    }

    if (splashscreen_texture != NULL) {
        SDL_DestroyTexture(splashscreen_texture);
        splashscreen_texture = NULL;
    }
}

void game::cleanup_sdl(void) {
    TTF_CloseFont(game_font);
    game_font = NULL;
    SDL_DestroyRenderer(game_renderer);
    SDL_DestroyWindow(game_window);
    game_window = NULL;
    game_renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

game::game(unsigned window_width, unsigned window_height, std::string window_title) {
    game_window_width = window_width;
    game_window_height = window_height;
    game_window_title = window_title;
    
    init_sdl();
    init_game();
}

game::~game() {
    cleanup_game();
    cleanup_sdl();
}

// game input

void game::mouse_event(SDL_Event *e) {
    // mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ) {
        // mouse is clicked
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            mouse_clicked = true;
        } else {
            mouse_clicked = false;
        }

        // mouse is moved
        if (e->type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&mouse_x, &mouse_y);
        }
    }
}

void game::keyboard_event(SDL_Event *e) {
    if (e->type == SDL_KEYDOWN) {
        // check for escape key press
        if ((e->key.keysym.sym == SDLK_ESCAPE) && game_state == GAME_STATE::GAME) { // enables user to use settings in game and to exit their game
            game_state = GAME_STATE::SETTINGS;
            init_settings_menu();
        }
    }
}

/*
    Update Code
*/

void game::update_main_menu(void) {

    // check if an item has been selected
    for (unsigned i = 0; i < main_menu->get_items(); i++) {
        if (main_menu->check_button(mouse_x, mouse_y, &main_menu->item_boundaries[i])) {
            main_menu->item_checked[i] = true;
            if (mouse_clicked == true) {
                game_state = main_menu->item_game_state[i];
            }
        } else {
            main_menu->item_checked[i] = false;
        }
    }
}

void game::update_settings_menu(void) {
    for (unsigned i = 0; i < settings_menu->get_items(); i++) {
        // mouse is over an item
        if (settings_menu->check_button(mouse_x, mouse_y, &settings_menu->item_boundaries[i])) {
            settings_menu->item_checked[i] = true;
            // mouse is over an item and has been clicked
            if (mouse_clicked == true) {
                if (settings_menu->item_settings[i] == ITEM_SETTINGS::MENU) {
                    game_state = GAME_STATE::MENU;
                    init_main_menu();
                }
            }
        } else {
            settings_menu->item_checked[i] = false;
        }
    }
}

void game::update_splash_screen(double dt) {
    if (((int)dt) > SPLASHSCREEN_TIME) { // how long splash screen is displayed for
        game_state = GAME_STATE::MENU;
        printf("Splash screen ended at %G ms\n", dt);
        splashscreen_timer->pause_timer();
        splashscreen_timer->reset_timer(); // might be redundant, could remove?
    }
}

void game::update_game(void) {

}

/*
    Render Code
*/

void game::render_splash_screen(void) {
    SDL_SetRenderDrawColor(game_renderer, background_color.r, background_color.g, background_color.b, background_color.a);
    SDL_RenderClear(game_renderer);

    SDL_RenderCopy(game_renderer, game_texture_manager->get_texture("splashscreen_texture"), NULL, &game_texture_manager->texture_quads["splashscreen_texture"]);
    SDL_RenderCopy(game_renderer, splashscreen_texture, NULL, &splashscreen_text_quad);
}

void game::render_main_menu(void) {
    SDL_SetRenderDrawColor(game_renderer, background_color.r, background_color.g, background_color.b, background_color.a);
    SDL_RenderClear(game_renderer);
    SDL_RenderCopy(game_renderer, version_info, NULL, &display_version_info);

    for (unsigned i = 0; i < main_menu->get_items(); i++) {
        if (main_menu->item_checked[i] == false) {
            SDL_RenderCopy(game_renderer, main_menu->item_textures[i], NULL, &main_menu->item_boundaries[i]);
        } else if (main_menu->item_checked[i] == true) {
            SDL_RenderCopy(game_renderer, main_menu->selected_item_textures[i], NULL, &main_menu->item_boundaries[i]);
        }
    }
}

void game::render_settings_menu(void) {
    SDL_SetRenderDrawColor(game_renderer, background_color.r, background_color.g, background_color.b, background_color.a);
    SDL_RenderClear(game_renderer);
    SDL_RenderCopy(game_renderer, version_info, NULL, &display_version_info);

    for (unsigned i = 0; i < settings_menu->get_items(); i++) {
        if (settings_menu->item_checked[i] == false) {
            SDL_RenderCopy(game_renderer, settings_menu->item_textures[i], NULL, &settings_menu->item_boundaries[i]);
        } else if (settings_menu->item_checked[i] == true) {
            SDL_RenderCopy(game_renderer, settings_menu->selected_item_textures[i], NULL, &settings_menu->item_boundaries[i]);
        }
    }
}

void game::render_game(void) {
    // set background color of window
    SDL_SetRenderDrawColor(game_renderer, background_color.r, background_color.g, background_color.b, background_color.a);
    SDL_RenderClear(game_renderer);

    // render version info at bottom of screen
    SDL_RenderCopy(game_renderer, version_info, NULL, &display_version_info);

    // render board boundary
    SDL_SetRenderDrawColor(game_renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(game_renderer, &board_boundary);

    // render chess board
    SDL_RenderCopy(game_renderer, game_texture_manager->get_texture("chess_board"), NULL, &game_texture_manager->texture_quads["chess_board"]);
}

// main game loop

void game::loop(void) {
    bool quit = false;
    SDL_Event e;

    while (!quit) { // main app loop (everything is handled in here...)
        frame_start = SDL_GetTicks64(); // used in fps limiting
        while (SDL_PollEvent(&e) != 0) { // input loop
            if ((e.type == SDL_QUIT) | (game_state == GAME_STATE::QUIT)) {
                quit = true;
            }
            mouse_event(&e);
            keyboard_event(&e);
        }

        if (game_state == GAME_STATE::SPLASH_SCREEN) {
            update_splash_screen(splashscreen_timer->get_time());
            render_splash_screen();
        }

        if (game_state == GAME_STATE::MENU) {
            // update and refresh game menu
            update_main_menu();
            render_main_menu();
        } 

        if (game_state == GAME_STATE::GAME) {
            // all game logic will be in here
            update_game();
            render_game();
        }

        if (game_state == GAME_STATE::SETTINGS) {
            // go to settings submenu
            update_settings_menu();
            render_settings_menu();
        }

        // update screen
        SDL_RenderPresent(game_renderer);

        // fps limiting
        frame_time = SDL_GetTicks64() - frame_start;

        if (FRAME_DELAY > frame_time) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }
}
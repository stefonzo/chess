#include "game.h"

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
    Game Code
*/

void game::init_game(void) {
    printf("Intializing game...\n");

    // initialize text color
    main_color.r = 0;
    main_color.g = 0;
    main_color.b = 0;
    main_color.a = 255;

    // initialize text select color
    selected_color.r = 255;
    selected_color.g = 255;
    selected_color.b = 255;
    selected_color.a = 255;
    
    VERSION = "Version 0.01";

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
    display_version_info.w = VERSION.length() * CHAR_WIDTH;
    display_version_info.h = 25;

    init_main_menu();
    init_settings_menu();
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

// menu initialization
void game::init_main_menu(void) {
    mouse_clicked = false;
    game_state = GAME_STATE::MENU;
    main_menu = std::make_unique<menu>(3);

    // initialize main menu text (crash occurs here)
    main_menu->menu_items[0] = "Play";
    main_menu->menu_items[1] = "Settings";
    main_menu->menu_items[2] = "Exit";

    // associate game states with different buttons
    main_menu->item_game_state[0] = GAME_STATE::GAME;
    main_menu->item_game_state[1] = GAME_STATE::SETTINGS;
    main_menu->item_game_state[2] = GAME_STATE::QUIT;
    /*
        Main Menu Initialization   
    */

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
        main_menu->item_boundaries[i].w = main_menu->menu_items[i].length() * CHAR_WIDTH;
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
    /*
        Settings Menu Initialization
    */

   settings_menu = std::make_unique<menu>(1); // for now there aren't any settings until I begin programming the game...
   settings_menu->menu_items[0] = "Back";
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
        settings_menu->item_boundaries[i].w = main_menu->menu_items[i].length() * CHAR_WIDTH;
        settings_menu->item_boundaries[i].h = 30;
   }

   // initialize if settings are checked (false by default) (put this in constructor...)
   for (unsigned i = 0; i < settings_menu->get_items(); i++) {
        settings_menu->item_checked[i] = false;
   }
}

// cleanup textures and other resources used in game
void game::cleanup_game(void) {
    main_menu->cleanup_menu();
    settings_menu->cleanup_menu();

    if (version_info != NULL) {
        SDL_DestroyTexture(version_info);
        version_info = NULL;
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

void game::mouse_event(SDL_Event *e) {
    // mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP ) {
        // mouse is clicked
        if (e->type == SDL_MOUSEBUTTONDOWN) {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            mouse_clicked = true;
            printf("x: %d y: %d\n", mouse_x, mouse_y);
        } else {
            mouse_clicked = false;
        }

        // mouse is moved
        if (e->type == SDL_MOUSEMOTION) {
            SDL_GetMouseState(&mouse_x, &mouse_y);
            printf("x: %d y: %d\n", mouse_x, mouse_y);
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

void game::render_main_menu(void) {
    SDL_SetRenderDrawColor(game_renderer, 0, 220, 50, 255);
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

void game::render_settings_menu(void) {
    SDL_SetRenderDrawColor(game_renderer, 0, 220, 50, 255);
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

void game::update_game(void) {

}

void game::render_game(void) {
    SDL_SetRenderDrawColor(game_renderer, 0, 220, 50, 255);
    SDL_RenderClear(game_renderer);
    SDL_RenderCopy(game_renderer, version_info, NULL, &display_version_info);
}

void game::loop(void) {
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if ((e.type == SDL_QUIT) | (game_state == GAME_STATE::QUIT)) {
                quit = true;
            }
            mouse_event(&e);
            keyboard_event(&e);
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

        printf("Game state: %i\n", game_state);
        
        // update screen
        SDL_RenderPresent(game_renderer);
    }
}
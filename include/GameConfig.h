#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int TILE_SIZE = 32;
const int GRID_WIDTH = WINDOW_WIDTH / TILE_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / TILE_SIZE;

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

#endif // GAME_CONFIG_H
#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Player.h"
#include "Bullet.h"

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void handleEvents();
    void update();
    void render();
    void clean();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* shipTexture;
    bool isRunning;
    Player* player;
    std::vector<Bullet> bullets;
    int fireCooldown;
};

#endif


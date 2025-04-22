#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

using namespace std;

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void handleEvents();
    void update(float deltaTime);
    void render();
    void clean();
    bool checkCollision(SDL_Rect a, SDL_Rect b);
    void drawHealthBar(int x, int y, int width, int height, int health, int maxHealth);
    SDL_Renderer* getRenderer() {
        return renderer;
    }
    void renderScore();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* shipTexture;
    SDL_Texture* backgroundTexture;
    SDL_Texture* bulletTexture;
    SDL_Texture* enemyTexture;
    TTF_Font* font;
    SDL_Texture* scoreTexture;
    bool isRunning;
    Player* player;
    vector<Bullet> bullets;
    int fireCooldown;
    vector<Enemy> enemies;
    int enemySpawnCooldown;
    int score;
};

#endif


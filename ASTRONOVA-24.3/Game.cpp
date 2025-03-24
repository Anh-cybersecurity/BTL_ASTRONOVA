#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int FIRE_RATE = 350;
const int ENEMY_COOLDOWN = 1000;

Game::Game() : window(nullptr), renderer(nullptr), shipTexture(nullptr), isRunning(false), player(nullptr), fireCooldown(0) {}

Game::~Game() {}

bool Game::init() {
    srand(time(0));
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << SDL_GetError();
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << IMG_GetError();
        SDL_Quit();
        return false;
    }

    window = SDL_CreateWindow("Astro Nova", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << SDL_GetError();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << SDL_GetError();
        return false;
    }

    SDL_Surface* loadedSurface = IMG_Load("tiny_ship10.png");
    if (!loadedSurface) {
        cerr << IMG_GetError();
    } else {
        shipTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    if (!shipTexture) {
        cerr << SDL_GetError();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    SDL_Surface* bg = IMG_Load("Space Background.png");
    if (!bg) {
        cerr << "Failed to load background image! Error: " << IMG_GetError() << endl;
    } else {
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
        SDL_FreeSurface(bg);
    }

    if (!backgroundTexture) {
        cerr << "Failed to create texture from background image! Error: " << SDL_GetError() << endl;
    }

    SDL_Surface* bullet = IMG_Load("bullet.png");
    if (!bullet) {
        cerr << "Failed to load bullet image! Error: " << IMG_GetError() << endl;
    } else {
        bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
        SDL_FreeSurface(bullet);
    }
    if (!bulletTexture) {
        cerr << "Failed to create texture from bullet image! Error: " << SDL_GetError() << endl;
    }

    SDL_Surface* enemySurface = IMG_Load("enemy.png");
    if (!enemySurface) {
        cerr << "Failed to load enemy image! Error: " << IMG_GetError() << endl;
    } else {
        enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
        SDL_FreeSurface(enemySurface);
    }
    enemySpawnCooldown = ENEMY_COOLDOWN;


    player = new Player(1000 / 2 - Player::WIDTH / 2, 800 - Player::HEIGHT - 20);
    isRunning = true;
    return true;
}

void Game::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            isRunning = false;
    }
    player->handleInput();
}

void Game::update() {
    player->update();

    if (fireCooldown > 0){
        fireCooldown--;
    }

    if (player->isFiring() && fireCooldown == 0) {
        bullets.push_back(Bullet(player->getX() + Player::WIDTH / 2 - Bullet::BULLET_WIDTH / 2, player->getY()));
        fireCooldown = FIRE_RATE;
    }
    for (auto& bullet : bullets)
        bullet.update();
    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.isOutOfBounds(); }), bullets.end());

    if (enemySpawnCooldown > 0) {
        enemySpawnCooldown--;
    } else {
        int spawnX = rand() % (1000 - Enemy::ENEMY_WIDTH);
        enemies.push_back(Enemy(spawnX, -Enemy::ENEMY_HEIGHT));
        enemySpawnCooldown = 1000;
    }

    for (auto& enemy : enemies) {
        enemy.update();
    }

    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return e.isOutOfBounds(); }), enemies.end());

}

void Game::render() {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);

    player->render(renderer, shipTexture);

    for (auto& bullet : bullets){
        bullet.render(renderer, bulletTexture);
    }

    for (auto& enemy : enemies) {
        enemy.render(renderer, enemyTexture);
    }

    SDL_RenderPresent(renderer);

}

void Game::clean() {
    delete player;

    SDL_DestroyTexture(shipTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if(bulletTexture){
        SDL_DestroyTexture(bulletTexture);
        bulletTexture = nullptr;
    }

    if (enemyTexture) {
        SDL_DestroyTexture(enemyTexture);
        enemyTexture = nullptr;
    }

    IMG_Quit();
    SDL_Quit();
}

#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int FIRE_RATE = 10;
const int ENEMY_COOLDOWN = 200;
const int FPS = 60;
const int FRAMERATE = 1000/60;

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
    Uint32 frameStart, frameTime;
    float deltaTime;
    while (isRunning) {
        frameStart = SDL_GetTicks();

        deltaTime = (SDL_GetTicks() - frameStart)/1000.0f;

        handleEvents();
        update(deltaTime);
        render();

        frameTime = SDL_GetTicks() - frameStart;

        if(FRAMERATE > frameTime){
            SDL_Delay(FRAMERATE - frameTime);
        }
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

bool Game::checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

void Game::update(float deltaTime) {
    player->update();

    if (fireCooldown > 0){
        fireCooldown--;
    }

    if (player->isFiring() && fireCooldown == 0) {
        bullets.push_back(Bullet(player->getX() + Player::WIDTH / 2 - Bullet::BULLET_WIDTH / 2, player->getY()));
        fireCooldown = FIRE_RATE;
    }
    for (auto& bullet : bullets) {
        bullet.update();
    }
        bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.isOutOfBounds(); }), bullets.end());

    if (enemySpawnCooldown <= 0) {
        int spawnExtra = SDL_GetTicks()/1000;
        int baseEnemies = 1;
        int numEnemies = rand() % (baseEnemies + spawnExtra) + 1;
        for (int i = 0; i < numEnemies; i++) {
            int spawnX = rand() % (SCREEN_WIDTH - Enemy::ENEMY_WIDTH);
            enemies.push_back(Enemy(spawnX, -Enemy::ENEMY_HEIGHT));
        }
        enemySpawnCooldown = ENEMY_COOLDOWN;
    } else {
        enemySpawnCooldown--;
    }

    for (auto& enemy : enemies) {
        enemy.update();
    }

    for (auto it = enemies.begin(); it != enemies.end();) {
        if (it->isOutOfBounds()) {
            isRunning = false;
            cout << "Enemy escaped! Game Over!" << endl;
            break;
        } else {
            ++it;
        }
    }

    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
    bool bulletHit = false;

    for (auto enemy = enemies.begin(); enemy != enemies.end();) {
        SDL_Rect bulletRect = { bullet->getX(), bullet->getY(), Bullet::BULLET_WIDTH, Bullet::BULLET_HEIGHT };
        SDL_Rect enemyRect = { enemy->getX(), enemy->getY(), Enemy::ENEMY_WIDTH, Enemy::ENEMY_HEIGHT };

        if (checkCollision(bulletRect, enemyRect)) {
            enemy = enemies.erase(enemy);
            bulletHit = true;
            break;
        } else {
            ++enemy;
        }
    }

    if (bulletHit) {
        bullet = bullets.erase(bullet);
        } else {
            ++bullet;
        }
    }

    SDL_Rect playerRect = { player->getX(), player->getY(), Player::WIDTH, Player::HEIGHT };

    for (auto enemy = enemies.begin(); enemy != enemies.end();) {
        SDL_Rect enemyRect = { enemy->getX(), enemy->getY(), Enemy::ENEMY_WIDTH, Enemy::ENEMY_HEIGHT };

        if (checkCollision(playerRect, enemyRect)) {
            player->health -= 33;
            enemy = enemies.erase(enemy);
            break;
        }

        if(player->health < 2){
            cout << "You have crashed! Game Over!" << endl;
            SDL_Delay(1);
            isRunning = false;
            return;
        } else {
            enemy++;
        }
    }
}

void Game::drawHealthBar(int x, int y, int width, int height, int health, int maxHealth) {
    SDL_Rect bgRect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    float healthPercentage = (float)health / maxHealth;
    SDL_Rect healthRect = {x, y, (int)(width * healthPercentage), height};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &healthRect);
}

void Game::render() {
    SDL_Rect bgRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect);

    player->render(renderer, shipTexture);

    drawHealthBar(player->getX(), player->getY() - 10, Player::WIDTH, 5, player->health, 100);

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

#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

const int FIRE_RATE = 85;

Game::Game() : window(nullptr), renderer(nullptr), shipTexture(nullptr), isRunning(false), player(nullptr), fireCooldown(0) {}

Game::~Game() {}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << SDL_GetError();
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << IMG_GetError();
        SDL_Quit();
        return false;
    }
    window = SDL_CreateWindow("Astro Nova", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, SDL_WINDOW_SHOWN);
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
    if (fireCooldown > 0)
        fireCooldown--;
    if (player->isFiring() && fireCooldown == 0) {
        bullets.push_back(Bullet(player->getX() + Player::WIDTH / 2 - Bullet::WIDTH / 2, player->getY()));
        fireCooldown = FIRE_RATE;
    }
    for (auto& bullet : bullets)
        bullet.update();
    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.isOutOfBounds(); }), bullets.end());
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    player->render(renderer, shipTexture);
    for (auto& bullet : bullets)
        bullet.render(renderer);
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    delete player;
    SDL_DestroyTexture(shipTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

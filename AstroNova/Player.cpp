#include "Player.h"
#include <SDL.h>
#include <cmath>
using namespace std;

Player::Player(int x, int y) : x(x), y(y), firing(false) {}

void Player::handleInput() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) {
        x -= SPEED;
    }
    if (keys[SDL_SCANCODE_D]) {
        x += SPEED;
    }
    if (keys[SDL_SCANCODE_W]) {
        y -= SPEED;
    }
    if (keys[SDL_SCANCODE_S]) {
        y += SPEED;
    }
    firing = keys[SDL_SCANCODE_SPACE];
}

void Player::update() {
    if (x < 0) x = 0;
    if (x > 1000 - WIDTH) x = 1000 - WIDTH;
    if (y < 0) y = 0;
    if (y > 800 - HEIGHT) y = 800 - HEIGHT;
}

void Player::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Rect playerRect = { x, y, WIDTH, HEIGHT };
    SDL_RenderCopy(renderer, texture, NULL, &playerRect);
}

bool Player::isFiring() {
    return firing;
}

int Player::getX() {
    return x;
}

int Player::getY() {
    return y;
}

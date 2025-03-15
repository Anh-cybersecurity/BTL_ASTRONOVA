#include "Player.h"
#include <SDL.h>
#include <cmath>
using namespace std;

Player::Player(int x, int y) : x(x), y(y), firing(false), angle(0) {}

void Player::handleInput() {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]){
        x -= SPEED;
        cout << "A was pressed \n";
    }
    if (keys[SDL_SCANCODE_D]) {
        x += SPEED;
        cout << "D was pressed \n";
    }
    if (keys[SDL_SCANCODE_W]) {
        y -= SPEED;
        cout << "W was pressed \n";
    }
    if (keys[SDL_SCANCODE_S]) {
        y += SPEED;
        cout << "S was pressed \n";
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
    SDL_Rect rect = { x, y, WIDTH, HEIGHT };
    SDL_Point center = { WIDTH / 2, HEIGHT / 2 };
    SDL_RenderCopyEx(renderer, texture, NULL, &rect, angle, &center, SDL_FLIP_NONE);
}

bool Player::isFiring() const {
    return firing;
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}


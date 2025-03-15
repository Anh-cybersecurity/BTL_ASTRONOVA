#include "Bullet.h"
#include <SDL.h>
using namespace std;

Bullet::Bullet(int x, int y) : x(x), y(y) {}

void Bullet::update() {
    y -= SPEED;
}

void Bullet::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { x, y, WIDTH, HEIGHT };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool Bullet::isOutOfBounds() const {
    return y < 0;
}


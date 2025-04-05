#include "Bullet.h"
#include <SDL.h>
using namespace std;

Bullet::Bullet(int x, int y) : x(x), y(y) {}

void Bullet::update() {
    y -= SPEED;
}

void Bullet::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Rect bulletRect = { x, y, BULLET_WIDTH, BULLET_HEIGHT };
    SDL_RenderCopy(renderer, texture, NULL, &bulletRect);
}

bool Bullet::isOutOfBounds() {
    return y + BULLET_HEIGHT < 0;
}

int Bullet::getX(){
    return x;
}

int Bullet::getY(){
    return y;
}

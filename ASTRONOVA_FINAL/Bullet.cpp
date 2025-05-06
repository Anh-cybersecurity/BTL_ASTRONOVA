#include "Bullet.h"
#include <SDL.h>
using namespace std;

Bullet::Bullet(int x, int y, int direction) : x(x), y(y), direction(direction) {}

void Bullet::update() {
	y += direction * SPEED;
}

void Bullet::render(SDL_Renderer* renderer, SDL_Texture* texture) {
	SDL_Rect bulletRect = { x, y, BULLET_WIDTH, BULLET_HEIGHT };
	SDL_RenderCopy(renderer, texture, NULL, &bulletRect);
}

bool Bullet::isOutOfBounds() {
	return (y + BULLET_HEIGHT < 0 || y + BULLET_HEIGHT >800);
}

int Bullet::getX()const {
	return x;
}

int Bullet::getY()const {
	return y;
}

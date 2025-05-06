#include "Enemy.h"

Enemy::Enemy(int x, int y) : lastShootTime(0), x(x), y(y) {}

void Enemy::update() {
	y += ENEMY_SPEED;
}

void Enemy::render(SDL_Renderer* renderer, SDL_Texture* enemyTexture) {
	SDL_Rect enemyRect = { x, y, ENEMY_WIDTH, ENEMY_HEIGHT };
	SDL_RenderCopy(renderer, enemyTexture, NULL, &enemyRect);
}

bool Enemy::isOutOfBounds() {
	return y > 800;
}

int Enemy::getX()const {
	return x;
}

int Enemy::getY()const {
	return y;
}

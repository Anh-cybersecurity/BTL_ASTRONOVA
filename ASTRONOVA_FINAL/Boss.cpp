#include "Boss.h"
#include <SDL.h>
#include <vector>
#include "Bullet.h"

using namespace std;

const int BOSS_WIDTH = 250;
const int BOSS_HEIGHT = 250;

Boss::Boss(int x, int y)
	: health(100), x(x), y(y), speed(1), state(BossState::Idle), lastActionTime(SDL_GetTicks()),
	chargingTime(10), firingTime(1000)
{
}

void Boss::update() {
	y += speed;

	Uint32 now = SDL_GetTicks();

	switch (state) {
	case BossState::Idle:
		if (now - lastActionTime >= 5000) {
			state = BossState::Charging;
			lastActionTime = now;
		}
		break;

	case BossState::Charging:
		if (now - lastActionTime >= chargingTime) {
			state = BossState::Firing;
			lastActionTime = now;
		}
		break;

	case BossState::Firing:
		if (now - lastActionTime >= firingTime) {
			state = BossState::Idle;
			lastActionTime = now;
		}
		break;
	}
}

void Boss::render(SDL_Renderer* renderer, SDL_Texture* bossTexture) {
	SDL_Rect bossRect = { x, y, BOSS_WIDTH, BOSS_HEIGHT };
	SDL_RenderCopy(renderer, bossTexture, NULL, &bossRect);
}

void Boss::fireLaser(vector<Bullet>& bossLasers) {
	if (state == BossState::Firing) {
		int laserX = x + BOSS_WIDTH / 2 - Bullet::BULLET_WIDTH / 2;
		int laserY = y + BOSS_HEIGHT;
		bossLasers.push_back(Bullet(laserX, laserY, 1));
	}
}

bool Boss::isFiringLaser() {
	return state == BossState::Firing;
}

SDL_Rect Boss::getRect() {
	return { x, y, BOSS_WIDTH, BOSS_HEIGHT };
}

int Boss::getX() const {
	return x;
}

int Boss::getY() const {
	return y;
}

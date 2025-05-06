#ifndef BOSS_H
#define BOSS_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Bullet.h"

using namespace std;

enum class BossState {
	Idle,
	Charging,
	Firing
};

class Boss {
public:
	Boss(int x, int y);
	void update();
	void render(SDL_Renderer* renderer, SDL_Texture* texture);
	void fireLaser(vector<Bullet>& bullets);
	SDL_Rect getRect();
	int getX() const;
	int getY() const;
	int health;
	bool isFiringLaser();

private:
	int x, y;
	int speed;

	BossState state;
	Uint32 lastActionTime;
	Uint32 chargingTime;
	Uint32 firingTime;
};

#endif

#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
	static const int BULLET_WIDTH = 64;
	static const int BULLET_HEIGHT = 64;
	static const int SPEED = 20;
	static const int FIRE_RATE = 8;
	Bullet(int x, int y, int direction);
	void update();
	void render(SDL_Renderer* renderer, SDL_Texture* texture);
	bool isOutOfBounds();
	int getX()const;
	int getY()const;
private:
	int x, y;
	int direction;
};

#endif


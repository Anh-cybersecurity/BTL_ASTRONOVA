#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    static const int BULLET_WIDTH = 64;
    static const int BULLET_HEIGHT = 64;
    static const int SPEED = 20;
    static const int FIRE_RATE = 10;
    Bullet(int x, int y);
    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    bool isOutOfBounds();
    int getX();
    int getY();
private:
    int x, y;
};

#endif


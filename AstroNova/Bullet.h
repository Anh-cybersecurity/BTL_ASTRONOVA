#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    static const int BULLET_WIDTH = 64;
    static const int BULLET_HEIGHT = 64;
    static const int SPEED = 5;
    static const int FIRE_RATE = 350;
    Bullet(int x, int y);
    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    bool isOutOfBounds();
private:
    int x, y;
};

#endif


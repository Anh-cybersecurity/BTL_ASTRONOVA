#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    static const int WIDTH = 10;
    static const int HEIGHT = 50;
    static const int SPEED = 2;
    static const int FIRE_RATE = 150;
    Bullet(int x, int y);
    void update();
    void render(SDL_Renderer* renderer);
    bool isOutOfBounds() const;
private:
    int x, y;
};

#endif


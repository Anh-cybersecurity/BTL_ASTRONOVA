#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

class Enemy {
public:
    static const int ENEMY_WIDTH = 60;
    static const int ENEMY_HEIGHT = 60;
    static const int ENEMY_SPEED = 7;

    Enemy(int x, int y);
    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    bool isOutOfBounds();
    int getX();
    int getY();
    SDL_Rect getRect() {
        return {x, y, ENEMY_WIDTH, ENEMY_HEIGHT};
    }
private:
    int x, y;
};

#endif

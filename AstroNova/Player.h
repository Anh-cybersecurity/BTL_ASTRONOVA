#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <SDL.h>
#include <vector>
#include "Bullet.h"

class Player {
public:
    static const int WIDTH = 70;
    static const int HEIGHT = 70;
    static const int SPEED = 1;
    Player(int x, int y);
    void handleInput();
    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    bool isFiring() const;
    int getX() const;
    int getY() const;
private:
    int x, y;
    bool firing;
    double angle;
};

#endif


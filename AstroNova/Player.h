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
    static const int SPEED = 2;
    Player(int x, int y);
    void handleInput();
    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    bool isFiring();
    int getX();
    int getY();
private:
    int x, y;
    bool firing;
};

#endif


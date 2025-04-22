#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <SDL.h>
#include <vector>
#include "Bullet.h"

class Player {
public:
    static const int WIDTH = 60;
    static const int HEIGHT = 60;
    static const int SPEED = 15;
    Player(int x, int y);
    void handleInput();
    void update();
    void render(SDL_Renderer* renderer, SDL_Texture* texture);
    int health;
    bool isFiring();
    int getX();
    int getY();
private:
    int x, y;
    bool firing;
};

#endif


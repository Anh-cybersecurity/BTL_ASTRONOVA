#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <vector>
#include <string>

using namespace std;

class GameOverMenu {
public:
    GameOverMenu(SDL_Renderer* renderer);
    ~GameOverMenu();

    int show();

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    TTF_Font* font;
    SDL_Color textColor;
    vector<string> options;
    size_t selectedOption;

    void renderText(const string& text, int x, int y, bool selected);
    SDL_Texture* loadTexture(const string& path);
};

#endif

